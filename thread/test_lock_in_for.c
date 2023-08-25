#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
 * for循环中main获取锁后，再次获取会被阻塞
 *
 */

int main(){

  int i;
  pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
  for(i = 0; i < 3; i++){
   pthread_mutex_lock(&mut);
   printf("%d\n",i);

  }
  pthread_mutex_unlock(&mut);
  exit(0);
}
