#include "types.h"
#include "stat.h"
#include "user.h"

#define MAXFRAME 16
#define MAXREF 128
static int references[MAXREF] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};
static int nrefs = 20;

static int
simulate(int n, int lru, int verbose)
{
  int frame[MAXFRAME], stamp[MAXFRAME], i, j, slot, faults = 0, tick = 0;
  int hit, nextfifo = 0;
  for(i = 0; i < n; i++){ frame[i] = -1; stamp[i] = 0; }
  if(verbose) printf(1, "%s (%d frames):\n", lru ? "LRU" : "FIFO", n);
  for(i = 0; i < nrefs; i++){
    hit = 0; slot = -1;
    for(j = 0; j < n; j++)
      if(frame[j] == references[i]){ hit = 1; slot = j; break; }
    if(!hit){
      faults++;
      for(j = 0; j < n; j++) if(frame[j] == -1){ slot = j; break; }
      if(slot < 0){
        if(lru){
          slot = 0;
          for(j = 1; j < n; j++) if(stamp[j] < stamp[slot]) slot = j;
        } else {
          slot = nextfifo;
          nextfifo = (nextfifo + 1) % n;
        }
      }
      frame[slot] = references[i];
    }
    if(!hit || lru) stamp[slot] = ++tick;
    if(verbose){
      printf(1, "%d: %s [", references[i], hit ? "hit" : "fault");
      for(j = 0; j < n; j++) printf(1, "%s%d", j ? " " : "", frame[j]);
      printf(1, "]\n");
    }
  }
  return faults;
}

int
main(int argc, char **argv)
{
  int n = 3, i, f, l, argstart = 1, v;
  if(argc > 1){ n = atoi(argv[1]); argstart = 2; }
  if(n < 1 || n > MAXFRAME){ printf(1, "frames must be 1..%d\n", MAXFRAME); exit(); }
  if(argc > argstart){
    nrefs = 0;
    for(i = argstart; i < argc && nrefs < MAXREF; i++){
      v = atoi(argv[i]);
      references[nrefs++] = v;
    }
  }
  printf(1, "Reference string:");
  for(i = 0; i < nrefs; i++) printf(1, " %d", references[i]);
  printf(1, "\n");
  f = simulate(n, 0, 1); l = simulate(n, 1, 1);
  printf(1, "frames=%d FIFO faults=%d hits=%d hit-ratio=%d%%\n", n, f, nrefs-f, (nrefs-f)*100/nrefs);
  printf(1, "frames=%d LRU  faults=%d hits=%d hit-ratio=%d%%\n", n, l, nrefs-l, (nrefs-l)*100/nrefs);
  {
    printf(1, "\nAll frame counts:\nframes FIFO-faults FIFO-hit%% LRU-faults LRU-hit%%\n");
    for(i = 3; i <= 5; i++){
      f = simulate(i, 0, 0); l = simulate(i, 1, 0);
      printf(1, "%d %d %d%% %d %d%%\n", i, f, (nrefs-f)*100/nrefs, l, (nrefs-l)*100/nrefs);
    }
  }
  exit();
}
