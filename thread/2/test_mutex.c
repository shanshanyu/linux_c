#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


//具有外部链接的静态  全局变量
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//具有内部链接的静态  全局变量
int main(){


  static pthread_mutex_t mutex  = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_lock(&mutex);
  printf("hello\n");
  return 0;
}
