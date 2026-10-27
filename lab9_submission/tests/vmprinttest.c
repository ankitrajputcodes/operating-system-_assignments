#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
  char *heap;
  int i;
  printf(1, "=== initial page table (guard is mapped with U=0) ===\n");
  vmprint();
  printf(1, "=== after sbrk(8 pages) ===\n");
  heap = sbrk(8 * 4096);
  if(heap == (char*)-1)
    printf(1, "sbrk growth failed\n");
  else
    for(i = 0; i < 8; i++)
      heap[i * 4096] = i; // Touch each lazy page so it appears in the dump.
  vmprint();
  printf(1, "=== after sbrk(-1 page) ===\n");
  if(sbrk(-4096) == (char*)-1)
    printf(1, "sbrk shrink failed\n");
  vmprint();
  exit();
}
