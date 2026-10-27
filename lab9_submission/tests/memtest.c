#include "types.h"
#include "stat.h"
#include "user.h"
#include "meminfo.h"

static void show(char *label)
{
  struct meminfo m;
  if(getmeminfo(&m) < 0){
    printf(1, "getmeminfo failed\n");
    return;
  }
  printf(1, "%s: total=%d free=%d process=%d page-tables=%d\n",
         label, m.total_pages, m.free_pages, m.process_pages, m.page_table_pages);
}

int main(void)
{
  char *p;
  int before, pid;
  struct meminfo m;

  show("before");
  p = sbrk(8 * 4096);
  if(p == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }
  p[0] = 1;
  p[7 * 4096] = 2;
  show("after touching two pages");
  getmeminfo(&m);
  before = m.free_pages;
  pid = fork();
  if(pid == 0){
    p[3 * 4096] = 3;
    show("child after fork and touch");
    exit();
  }
  wait();
  show("after reaping child");
  getmeminfo(&m);
  printf(1, "reaped free pages %s (child-start=%d now=%d)\n",
         m.free_pages >= before ? "recovered" : "not recovered", before, m.free_pages);
  exit();
}
