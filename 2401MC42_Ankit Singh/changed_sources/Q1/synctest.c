#include "types.h"
#include "stat.h"
#include "user.h"

void
worker(int id, char letter)
{
  int i;
  for(i = 0; i < 5; i++){
    sem_wait(id);
    printf(1, "%c", letter);
    sem_post((id + 1) % 3);
  }
  exit();
}

int
main(void)
{
  int pid;
  sem_init(0, 1);
  sem_init(1, 0);
  sem_init(2, 0);
  if((pid = fork()) == 0) worker(0, 'A');
  if(pid > 0 && fork() == 0) worker(1, 'B');
  if(pid > 0 && fork() == 0) worker(2, 'C');
  if(pid > 0){
    wait(); wait(); wait();
    printf(1, " Done\n");
  }
  exit();
}
