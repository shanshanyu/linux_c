#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

/*
 *测试当前环境能创建多少个线程
 *
 */

void *thr_func(void *arg){
  int i;
  printf("i address:%ld\n",&i);
  sleep(100);
  return NULL;
}

int main(){
  long int i = 0;
  pthread_t tid;
  int err;
  while(1){
    err = pthread_create(&tid,NULL,thr_func,NULL);
    if(err != 0){
      fprintf(stderr,"pthread_create():%s\n",strerror(err));
      break;
    }
    i++;
  }
  printf("当前环境可以创建： %ld 个线程\n",i);
  err = pthread_join(tid,NULL);
  if(err != 0){
    fprintf(stderr,"pthread_join() failed:%s\n",strerror(err));
  }
  return 0;
}
