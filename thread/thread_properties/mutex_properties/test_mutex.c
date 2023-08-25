#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


int main(){
  printf("process shared:%d\n",sysconf(_SC_THREAD_PROCESS_SHARED));

  exit(0);
}
