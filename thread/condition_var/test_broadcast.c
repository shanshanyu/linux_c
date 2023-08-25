#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THRNUM 3

static int status = 0;
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *thr_func(void *arg){
  printf("thread %d running\n",(int)arg);
  int tmp = (int)arg;
  pthread_mutex_lock(&mut);
  printf("thread %d lock\n",(int)arg);
  while(status != 1){
    printf("1\n");
    pthread_cond_wait(&cond,&mut);
    printf("%d wait return\n",tmp);
    sleep(5);
  }
  pthread_mutex_unlock(&mut);
  pthread_exit(NULL);
}



int main(){
  

  pthread_t tid[THRNUM];
  int i;

  for(i = 0; i < THRNUM; i++)
    pthread_create(tid+i,NULL,thr_func,(void *)i);

  sleep(1);
  printf("create thread success\n");
  pthread_mutex_lock(&mut);
  printf("main running\n"); 
  status = 1;
  //pthread_cond_signal(&cond);
  pthread_cond_broadcast(&cond);
  pthread_mutex_unlock(&mut);

  for(i = 0; i < THRNUM; i++)
    pthread_join(tid[i],NULL);

  exit(0);
}
