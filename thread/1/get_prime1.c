#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

/*
 *计算 30000000 30000200 之前的质数，一共有 18 个
 * 使用多线程，一个数用一个线程
 *
 */
#define LEFT  30000000
#define RIGHT 30000200

static void *get_prime(void *arg){
  
  int i;
  int flag;
  int num = (int)arg;
  flag = 1;
  for(i = 2; i < num/2; i++)
    if(num % i == 0){
      flag = 0;
      break;
    }
  if(flag) printf("%d is primer\n",num);
  pthread_exit(NULL);
}
//主线程  main thread
int main(){
  int i;
  pthread_t tid_arr[RIGHT-LEFT+1];
  int err;
  //创建线程,需要把要计算的数传给线程
  for(i = LEFT; i <= RIGHT; i++){
    err = pthread_create(tid_arr+i-LEFT,NULL,get_prime,i);
    //线程创建失败打印错误信息
    if(err != 0){
      fprintf(stderr,"pthread_create():%s\n",strerror(err));
      exit(1);
    }
  }

  for(i = LEFT; i <= RIGHT; i++){
    err = pthread_join(tid_arr[i-LEFT],NULL);
    if(err != 0){
      fprintf(stderr,"pthread_join():%s\n",strerror(err));
      exit(1);
    }
  }
  return 0;
}

