#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
 *查看进程内可以创建多少个线程
 */

void *print_null(void *arg){
  ;
}

int main(){
  int i = 0;
  int err;
  pthread_t tid;
  while(1){
    err = pthread_create(&tid,NULL,print_null,NULL);
    if(err != 0) break;
    printf("%d\n",i++);
  }

  exit(0);
}
