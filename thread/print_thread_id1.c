#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


//打印控制线程的线程id，创建线程的线程id
//20230413


void *thr_func(void *arg){
  pthread_t tid;
  tid = pthread_self();
  printf("thread id = %ld \n",tid);
  return NULL;
}


int main(){
  pthread_t tid;
  int err;
  err = pthread_create(&tid,NULL,thr_func,NULL);
  if(err != 0){
    fprintf(stderr,"pthread_create(): %s\n",strerror(err));
  }
  printf("control thread id = %ld\n",pthread_self());
  pthread_join(tid,NULL);
  exit(0);
}
