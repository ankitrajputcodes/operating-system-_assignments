#include "types.h"
#include "stat.h"
#include "user.h"

int global_value = 2401;

int
main(void)
{
  int before, after_sbrk, parent_pa, child_pa, pid;
  before = freepages();
  printf(1, "freepages at start: %d\n", before);
  sbrk(10 * 4096);
  after_sbrk = freepages();
  printf(1, "freepages after sbrk(10 * 4096): %d\n", after_sbrk);
  printf(1, "global virtual=%d physical=%d\n", (int)&global_value,
         va2pa((int)&global_value));
  printf(1, "va2pa(0x7fffffff)=%d\n", va2pa(0x7fffffff));
  if((pid = fork()) == 0){
    printf(1, "child global virtual=%d physical=%d\n", (int)&global_value,
           va2pa((int)&global_value));
    exit();
  }
  parent_pa = va2pa((int)&global_value);
  wait();
  child_pa = va2pa((int)&global_value);
  printf(1, "parent global physical before/after wait=%d/%d\n",
         parent_pa, child_pa);
  printf(1, "freepages after fork and wait: %d\n", freepages());
  exit();
}
