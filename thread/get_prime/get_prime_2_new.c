#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define THR_NUM 4

#define LEFT 30000000
#define RIGHT 30000200

static int status = 0;
static int num = 0;
/*
 *status 三种状态：-1 线程准备退出，1 放数已经完成，等待worker取数，0已经取完数，等待放数
 *
 */


static void err_print(char *s,int err){
  fprintf(stderr,"%s err:%s\n",s,strerror(err));
  exit(1);
}

static void get_primer(int num){
  int flag = 1;
  int i;
  for(i = 2; i < num/2; i++){
    if(num % i == 0){
      flag = 0;
      break;
    } 
  }
  if(flag) printf("%d is primer\n",num);
}

static void *thr_func(void *arg){
  pthread_mutex_t *mut;
  mut = arg;
  int tmp;
  while(1){
    pthread_mutex_lock(mut);
    while(status == 0){
      pthread_mutex_unlock(mut);
      sched_yield();
      pthread_mutex_lock(mut);
    }
    if(status == -1){
      pthread_mutex_unlock(mut);
      pthread_exit(NULL);
    }
   tmp = num;
   status = 0;
   pthread_mutex_unlock(mut);
   get_primer(tmp);
  }
 
  pthread_exit(NULL);
}


int main(){
  pthread_t tid[THR_NUM];
  int err;
  pthread_mutex_t mut;
  pthread_mutex_init(&mut,NULL);
  num = LEFT;
  int i;
  //创建线程
  for(i = 0; i < THR_NUM; i++){
    err = pthread_create(tid+i,NULL,thr_func,&mut);
    if(err != 0)
      err_print("pthread_create",err);
  }

  while(num <= RIGHT){
    pthread_mutex_lock(&mut);
    if(status == 0){
      status = 1;
      num++;   
    }
    pthread_mutex_unlock(&mut);
  }


  pthread_mutex_lock(&mut);
  while(status == 1){
    pthread_mutex_unlock(&mut);
    sched_yield();
    pthread_mutex_lock(&mut);
  } 
  status = -1;
  pthread_mutex_unlock(&mut);

  for(i = 0; i < THR_NUM; i++)
   pthread_join(tid[i],NULL);

  exit(0);
}
