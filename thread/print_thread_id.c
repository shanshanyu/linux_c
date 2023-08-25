#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

/*
 *打印线程 id
 * 
 */
void print_tid(){
  printf("progress id = %lu,thread id = %lu\n",getpid(),pthread_self());
}

static void *thr_fn(void *arg){
  printf("i am new thread:");
  print_tid();
  //线程退出
  pthread_exit(NULL);
}



int main(){
  pthread_t tid;
  int err;
  //创建一个新的线程
  err = pthread_create(&tid,NULL,thr_fn,NULL);  
  if(err){
    fprintf(stderr,"pthread_create():%s\n",strerror(err));
    exit(1);
  }

  err = pthread_join(tid,NULL);
  if(err){
    fprintf(stderr,"pthread_join():%s\n",strerror(err)); 
  }

  printf("i am main thread:");
  print_tid();


  exit(0);
}
