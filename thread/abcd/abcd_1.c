#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
/*
 *20220613 review
 */

#define THR_NUM 4

static int status = 0;
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct thr_info{
  int index;
};

void *thr_func(void *arg){
  int i = ((struct thr_info *)arg)->index;
  char c = 'a'+i;
  while(1){
    pthread_mutex_lock(&mut);
    while(status != i)
      pthread_cond_wait(&cond,&mut);

    write(STDOUT_FILENO,&c,1);
    status = (status+1) % 4;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mut);
  }
  pthread_exit(NULL);
}



int main(){
  pthread_t  tid[THR_NUM];
  int err;
  int i;
  //创建 4 个线程
  for(i = 0; i < THR_NUM; i++){
    //在堆上创建空间
    struct thr_info *st;
    st = malloc(sizeof(struct thr_info));
    if(st == NULL){
      fprintf(stderr,"malloc failed\n");
      exit(1);
    }
    st->index = i;
    err = pthread_create(tid+i,NULL,thr_func,st);
    if(err != 0){
      fprintf(stderr,"pthread_create() failed\n");
      exit(1);
    }
  }
  alarm(5);
  for(i = 0; i < THR_NUM; i++){
    pthread_join(tid[i],NULL);
    printf("tid %d's pid = %ul",i,tid[i]);
  }
  return 0;
}

