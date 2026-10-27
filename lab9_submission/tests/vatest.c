#include "types.h"
#include "stat.h"
#include "user.h"

int global_value = 7;

int
main(void)
{
  int local_value = 11;
  char *heap_value = malloc(64);
  int pid;

  if(heap_value == 0){
    printf(1, "vatest: malloc failed\n");
    exit();
  }
  heap_value[0] = 'x';
  printf(1, "global va=0x%x pa=0x%x\n", (uint)(uint)&global_value, va2pa((int)&global_value));
  printf(1, "local  va=0x%x pa=0x%x\n", (uint)&local_value, va2pa((int)&local_value));
  printf(1, "heap   va=0x%x pa=0x%x\n", heap_value, va2pa((int)heap_value));
  printf(1, "unmapped 0x7fffffff -> %d\n", va2pa(0x7fffffff));
  pid = fork();
  if(pid < 0){
    printf(1, "vatest: fork failed\n");
    exit();
  }
  if(pid == 0){
    printf(1, "child  global va=0x%x pa=0x%x\n", (uint)(uint)&global_value, va2pa((int)&global_value));
    exit();
  }
  wait();
  printf(1, "parent global va=0x%x pa=0x%x\n", (uint)(uint)&global_value, va2pa((int)&global_value));
  free(heap_value);
  exit();
}
