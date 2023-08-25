#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int main(){

  printf("%ld\n",sysconf(_SC_THREAD_SAFE_FUNCTIONS));
  exit(0);
}
