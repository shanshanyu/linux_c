#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_func1(void *arg){
  struct timespec tv;
  tv.tv_sec = 1;
  tv.tv_nsec = 0;
  while(1){
	  if(nanosleep(&tv,NULL) == -1)
		  printf("nanosleep() error\n");
	  printf("func1 sleep over,running\n");
  }
  return NULL;

}
void *thr_func2(void *arg){
  struct timespec tv;
  tv.tv_sec = 5;
  tv.tv_nsec = 0;
  while(1){
	  if(nanosleep(&tv,NULL) == -1)
		  printf("nanosleep() error\n");
	  printf("func2 sleep over,running\n");
  }
  return NULL;

}

int main(){
  pthread_t tid1,tid2;
  pthread_create(&tid1,NULL,thr_func1,NULL);
  pthread_create(&tid2,NULL,thr_func2,NULL);
 
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
  exit(0);
}
