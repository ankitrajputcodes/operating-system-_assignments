#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char *oldend;
  int before;
  printf(1, "=== startup page table ===\n");
  vmprint();
  oldend = sbrk(0);
  sbrk(4 * 4096);
  // Touch one byte in each reserved page, then confirm four mappings appeared.
  before = (int)oldend;
  *(volatile char*)before = 1;
  *(volatile char*)(before + 4096) = 2;
  *(volatile char*)(before + 2 * 4096) = 3;
  *(volatile char*)(before + 3 * 4096) = 4;
  printf(1, "=== after sbrk(4 * 4096) and touching each page ===\n");
  vmprint();
  exit();
}
