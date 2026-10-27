#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char *p;
  int i, pid;
  p = sbrk(32 * 4096);
  if(p == (char*)-1){ printf(1, "reservation failed\n"); exit(); }
  for(i = 0; i < 5; i++)
    p[i * 3 * 4096] = i + 1;
  printf(1, "handled page faults: %d (expected 5)\n", getfaults());
  pid = fork();
  if(pid == 0){ *(volatile char*)(p + 33 * 4096) = 1; exit(); }
  if(pid > 0){ wait(); printf(1, "above-size child killed; parent survived\n"); }
  exit();
}
