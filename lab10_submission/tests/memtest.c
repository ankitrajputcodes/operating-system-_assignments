#include "types.h"
#include "stat.h"
#include "user.h"

static void
show(char *label)
{
  printf(1, "\n%s\n", label);
  if(memstats() < 0)
    printf(1, "memstats failed\n");
}

int
main(void)
{
  char *p;

  show("before allocation");
  p = sbrk(4 * 4096);
  if(p == (char*)-1){
    printf(1, "sbrk allocation failed\n");
    exit();
  }
  // Touch each page so the allocator supplies physical memory.
  p[0] = 1;
  p[4096] = 2;
  p[2 * 4096] = 3;
  p[3 * 4096] = 4;
  show("after allocating and touching four pages");

  if(sbrk(-4 * 4096) == (char*)-1)
    printf(1, "sbrk release failed\n");
  show("after releasing the four pages");
  exit();
}
