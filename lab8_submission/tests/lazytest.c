#include "types.h"
#include "stat.h"
#include "user.h"
#include "meminfo.h"

static void show(char *label)
{
  struct meminfo m;
  getmeminfo(&m);
  printf(1, "%s: free=%d user-pages=%d page-tables=%d\n",
         label, m.free_pages, m.process_pages, m.page_table_pages);
}

int main(void)
{
  char *region, *untouched, *readbuf;
  int i, p[2], pid, wrote;

  show("before reservation");
  region = sbrk(32 * 1024 * 1024);
  if(region == (char*)-1){
    printf(1, "32 MB reservation failed\n");
    exit();
  }
  show("after 32 MB reservation");
  for(i = 0; i < 32; i += 5)
    region[i * 1024 * 1024] = i;
  show("after touching seven sparse pages");

  untouched = sbrk(4096);
  if(untouched == (char*)-1){
    printf(1, "buffer reservation failed\n");
    exit();
  }
  // write() reads this page from kernel mode, exercising lazy fault handling.
  printf(1, "kernel write from untouched buffer: ");
  wrote = write(1, untouched, 1);
  printf(1, "\nwrite result=%d; translated page=%x\n", wrote, va2pa((int)untouched));

  pipe(p);
  if(fork() == 0){
    close(p[0]);
    write(p[1], "R", 1);
    close(p[1]);
    exit();
  }
  close(p[1]);
  readbuf = sbrk(4096);
  if(readbuf == (char*)-1 || read(p[0], readbuf, 1) != 1)
    printf(1, "read into untouched buffer failed\n");
  else
    printf(1, "kernel read into untouched buffer: %c\n", *readbuf);
  close(p[0]);
  wait();

  show("parent before lazy-page fork");
  pid = fork();
  if(pid == 0){
    region[18 * 1024 * 1024] = 9;
    show("child after touching inherited lazy page");
    exit();
  }
  if(pid > 0)
    wait();
  show("parent after child exit");

  pid = fork();
  if(pid == 0){
    // Address is within the 32 MB reservation plus two buffers, but beyond sz.
    *(volatile int*)(region + 33 * 1024 * 1024) = 1;
    printf(1, "invalid address unexpectedly survived\n");
    exit();
  }
  if(pid > 0){
    wait();
    printf(1, "out-of-range child was killed; parent remains alive\n");
  }
  pid = fork();
  if(pid == 0){
    printf(1, "guard-page probe\n");
    *(volatile int*)0x1000 = 1;
    printf(1, "guard-page child unexpectedly survived\n");
    exit();
  }
  if(pid > 0){
    wait();
    printf(1, "guard-page child was killed; parent remains alive\n");
  }
  printf(1, "lazy allocation/fork/buffer checks complete\n");
  exit();
}
