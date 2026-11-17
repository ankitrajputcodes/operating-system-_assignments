#include "types.h"
#include "param.h"
#include "mmu.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"

#define NSEMS 8

struct semaphore {
  int value;
  int inuse;
};

static struct {
  struct spinlock lock;
  struct semaphore sem[NSEMS];
} semtable;

void
seminit(void)
{
  int i;
  initlock(&semtable.lock, "semtable");
  for(i = 0; i < NSEMS; i++){
    semtable.sem[i].value = 0;
    semtable.sem[i].inuse = 0;
  }
}

int
sem_init(int id, int value)
{
  if(id < 0 || id >= NSEMS || value < 0)
    return -1;
  acquire(&semtable.lock);
  semtable.sem[id].value = value;
  semtable.sem[id].inuse = 1;
  release(&semtable.lock);
  return 0;
}

int
sem_wait(int id)
{
  struct semaphore *s;
  if(id < 0 || id >= NSEMS)
    return -1;
  acquire(&semtable.lock);
  s = &semtable.sem[id];
  if(!s->inuse){
    release(&semtable.lock);
    return -1;
  }
  while(s->value == 0){
    if(myproc()->killed){
      release(&semtable.lock);
      return -1;
    }
    sleep(&s->value, &semtable.lock);
  }
  s->value--;
  release(&semtable.lock);
  return 0;
}

int
sem_post(int id)
{
  struct semaphore *s;
  if(id < 0 || id >= NSEMS)
    return -1;
  acquire(&semtable.lock);
  s = &semtable.sem[id];
  if(!s->inuse){
    release(&semtable.lock);
    return -1;
  }
  s->value++;
  wakeup(&s->value);
  release(&semtable.lock);
  return 0;
}
