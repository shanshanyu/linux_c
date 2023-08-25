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
 *
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
     //Q:status == -1 的判断放在这里为什么会造成死锁？
     //A:不是死锁，程序的逻辑不对了，如果线程跑到了 while 循环，就会一致在那循环等
     if(status == -1) {
       pthread_exit(NULL);
     }
     //add lock
    err = pthread_mutex_lock(p);
    while(status != 1){
      err = pthread_mutex_unlock(p);
      sched_yield();
      //临界区开始
      err = pthread_mutex_lock(p);
   }
   status = 0;
   j = num;
   //unlock
   err = pthread_mutex_unlock(p);
   //临界区结束   

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
