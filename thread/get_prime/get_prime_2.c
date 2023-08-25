#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define LEFT 30000000
#define RIGHT 30000200


//use psedo thread pool
/*   status 3 种状态：
 *   -1：没有数需要计算，线程退出
 *   0: 等待放数
 *   1: 数已经放好，等待计算
 */
static int status = 0;
static int num;



static void *print_prime(void *p){
  int i;
  int flag;
  while(1){
    pthread_mutex_lock(p);
    //计算过程中不应该占用锁
    if(status == 1){
       status = 0;
       flag = 1;
       for(i = 2; i < num/2; i++)
         if(num % i == 0){
           flag = 0;
           break;
         }
      if(flag) printf("%d  is primer\n",num);
   }
   pthread_mutex_unlock(p);

   if(status == -1){
     pthread_exit(NULL);
   }
  }
}


int main(){
  pthread_mutex_t mut;
  pthread_mutex_init(&mut,NULL);
  int err;
  int i;
  int tmp;
  tmp = LEFT; 
  //创建 3 个线程
  pthread_t tid[3];
  for(i = 0; i < 3; i++){ 
    pthread_create(tid+i,NULL,print_prime,&mut);
  }


  while(1){
    //查看 status 的时候需要加锁，status 是共享资源
    pthread_mutex_lock(&mut);
    if(status == 0){
      num = tmp++;
 //     printf("put %d\n",num);
      status = 1;
    }
    pthread_mutex_unlock(&mut);
    
    //数放完了，等待线程计算完成后，将status置成-1
    if(tmp > RIGHT){
      pthread_mutex_lock(&mut);
      while(status != 0){
        pthread_mutex_unlock(&mut);
        sched_yield();
        pthread_mutex_lock(&mut);  
      }
      status = -1;
      pthread_mutex_unlock(&mut);
      break;
    }    
  }


  for(i = 0; i < 3; i++){
    pthread_join(tid[i],NULL);
  }

  exit(0);
}
