#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "my_pipe.h"
#include "op_pipe.h"


void *thr_func1(void *arg){
  int err;
  char buf[5];
  err = open_pipe(1,PIPE_READER);
  err = read_pipe_new(1,buf,5);
  printf("read size = %d\n",err);
  if(err <= 0)
    pthread_exit(NULL);

  puts(buf);
  return NULL;
}

void *thr_func2(void *arg){
  int err;
  char *str = "hello";
  err = open_pipe(1,PIPE_WRITER);
  err = write_pipe_new(1,str,5);
  if(err < 0)
    pthread_exit(NULL);

  return NULL;
}


int main(){

  pthread_t tid1,tid2;
 
  pthread_create(&tid1,NULL,thr_func1,NULL);
  pthread_create(&tid2,NULL,thr_func2,NULL);

  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
  

  return 0;
}
