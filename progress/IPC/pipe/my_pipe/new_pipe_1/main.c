#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "my_pipe.h"


void *thr_func1(void *arg){
  int err;
  char buf[5];
  struct my_pipe_st *p = arg;  
  register_client(p,PIPE_READER); 
  err = read_pipe(p,buf,5);
  printf("read size = %d\n",err);
  if(err <= 0)
    pthread_exit(NULL);

  puts(buf);
  return NULL;
}

void *thr_func2(void *arg){
  int err;
  char *str = "hello";
  struct my_pipe_st *p = arg;  
  register_client(p,PIPE_WRITER);
  err = write_pipe(p,str,5);
  if(err < 0)
    pthread_exit(NULL);

  return NULL;
}


int main(){

  pthread_t tid1,tid2;
  struct my_pipe_st *p = create_pipe(); 

  pthread_create(&tid1,NULL,thr_func1,p);
  pthread_create(&tid2,NULL,thr_func2,p);

  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
  

  return 0;
}
