#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void *thr_func(void *arg){
  return NULL;

}


int main(){

  pthread_t tid;
  int i;
  int err;
  while(1){
    err = pthread_create(&tid,NULL,thr_func,NULL);
    if(err != 0)
      break;
    i++;
  } 
  printf("max thread_num = %d\n",i);
  return 0;
}
