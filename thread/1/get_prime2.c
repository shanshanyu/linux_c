#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*
 *计算 30000000 30000200 之前的质数，一共有 18 个
 * 使用多线程，一个数用一个线程
 *这个程序有两个 warning，去掉warning
 */
#define LEFT  30000000
#define RIGHT 30000200



static void *get_prime(void *arg){
  
  int i;
  int flag;
  flag = 1;
  int *tmp = arg;
  int num = *tmp;
  for(i = 2; i < num/2; i++)
    if(num % i == 0){
      flag = 0;
      break;
    }
  if(flag) printf("%d is primer\n",num);
  pthread_exit(arg);
}
//主线程  main thread
int main(){
  int i;
  int *thr_arg;
  pthread_t tid_arr[RIGHT-LEFT+1];
  int err;
  void *tret;
  //创建线程,需要把要计算的数传给线程
  for(i = LEFT; i <= RIGHT; i++){
    //传递一个指针给新的线程，所以需要在 main thread 中 malloc
    //为什么不一次性分配好，而是每次分配一个？便于内存释放？
    thr_arg = malloc(sizeof(int));
    *thr_arg = i;
    err = pthread_create(tid_arr+i-LEFT,NULL,get_prime,thr_arg);
    //线程创建失败打印错误信息
    if(err != 0){
      fprintf(stderr,"pthread_create():%s\n",strerror(err));
      exit(1);
    }
  }
 
  //使用pthread_join 回收线程资源
  for(i = LEFT; i <= RIGHT; i++){
    err = pthread_join(tid_arr[i-LEFT],&tret);
    if(err != 0){
      fprintf(stderr,"pthread_join():%s\n",strerror(err));
      exit(1);
    }
    //释放malloc 的空间
    free(tret);
  }
  return 0;
}

