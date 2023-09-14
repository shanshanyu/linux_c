#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>


int main(){
  struct timespec tsp;
  clock_gettime(CLOCK_REALTIME,&tsp);

  printf("s = %d\n",tsp.tv_sec);
  printf("ns = %d\n",tsp.tv_nsec);

  exit(0);
}
