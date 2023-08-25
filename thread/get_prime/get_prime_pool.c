#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>
#define THRNUM 4
#define LEFT 30000000
#define RIGHT 30000200

static int status = 0;
static int num = 0;

/*
 *thread ppol   lock
 *
 *这个程序有个确定，会一直去加锁查数，很浪费时间，能否改成通知？
 */


static void *get_prime(void *p){
  int err;
  //add lock
  //get num
  //unlock
  //count
  int i,j;
  int flag;
  while(1){
    //add lock
    err = pthread_mutex_lock(p);
    while(status != 1){
      if(status == -1) {
       err = pthread_mutex_unlock(p);
       pthread_exit(NULL);
      }
      err = pthread_mutex_unlock(p);
      sched_yield();
      err = pthread_mutex_lock(p);
   }
   status = 0;
   //临时变量保存当前要计算的值
   j = num;
   //unlock
   err = pthread_mutex_unlock(p);
   
   flag = 1;
   for(i = 2; i < j/2; i++)
     if(j % i == 0){
        flag = 0;
        break;
     }
  if(flag) printf("%d is primer\n",j);

 }
}


int main(){
  pthread_mutex_t mut;
  int err;
  int i;
  pthread_t tid[THRNUM];
  
  err = pthread_mutex_init(&mut,NULL);

  //create THRNUM thread
  for(i = 0; i < THRNUM; i++){
    err = pthread_create(tid+i,NULL,get_prime,&mut);
  }
  //lock
  //put num
  //change status
  //unlock
  for(i = LEFT; i < RIGHT; i++){
    //add lock
    err = pthread_mutex_lock(&mut);
   
    while( status != 0){
      //unlock
      err = pthread_mutex_unlock(&mut);
      //return cpu
      sched_yield(); 
      //lock  ->  
      err = pthread_mutex_lock(&mut);
    }
    //put num
    num = i;
    //change status
    status = 1;
    err = pthread_mutex_unlock(&mut);
  } 
   
  //deal all num,change status -> -1
  err = pthread_mutex_lock(&mut);
  while(status != 0){
    err = pthread_mutex_unlock(&mut);
    sched_yield();
    err = pthread_mutex_lock(&mut);
  }
  status = -1;
  pthread_mutex_unlock(&mut);


  //reclaim thread
  for(i = 0; i < THRNUM; i++){
    err = pthread_join(tid[i],NULL);
  }
  exit(0);
}

/*
 *  status 有 3 种状态：
 *  1. 0：等待放数
 *  2. 1：数已经放好，等待线程去计算
 *  3. -1:线程退出
 */
