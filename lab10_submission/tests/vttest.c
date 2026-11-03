#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char *p;
  int i, pa;

  p = sbrk(2 * 4096);
  if(p == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }
  p[0] = 1;
  p[4096] = 2;

  for(i = 0; i < 2; i++){
    pa = vtop((int)(p + i * 4096));
    if(pa < 0)
      printf(1, "translation failed for va=0x%x\n", p + i * 4096);
    else
      printf(1, "va=0x%x -> pa=0x%x\n", p + i * 4096, pa);
  }

  pa = vtop(0x70000000);
  printf(1, "unmapped va=0x70000000 -> %d (expected -1)\n", pa);
  sbrk(-2 * 4096);
  exit();
}
