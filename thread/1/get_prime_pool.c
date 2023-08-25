#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*
 *计算 30000000 30000200 之前的质数，一共有 18 个
 * 使用多线程，池算法，分堆，每个线程处理一堆
 *这个程序有两个 warning，去掉warning
 */
#define LEFT  30000000
#define RIGHT 30000200

struct thr_info{
  int index;
  int thr_num;
};

void cleanup(void *arr){
  free(arr);
  printf("cleanup success\n");
}

//子线程
static void *get_prime(void *arg){
  
  int i,j;
  int flag;
  struct thr_info *th = arg;
  printf("thread running\n");
  //线程中需要线程个数 n
  for(j = LEFT+th->index; j <= RIGHT; j += th->thr_num){
  flag = 1;
  for(i = 2; i < j/2; i++)
    if(j % i == 0){
      flag = 0;
      break;
    }
  if(flag) printf("%d is primer\n",j);

  }
  //线程退出
  pthread_exit(th);
}
//主线程  main thread
int main(){
  printf("input how manager thread your want to create: ");
  void *tret;  
  int num;
  scanf("%d",&num);
  //创建一块空间放传递给子线程的参数
  struct thr_info *th = malloc(sizeof(struct thr_info)*num);
  if(th == NULL){
     fprintf(stderr,"malloc failed\n");
     exit(1);
   }
  //创建一块空间保存 tid
  pthread_t *tid_arr = malloc(sizeof(pthread_t)*num);
  if(tid_arr == NULL){
     fprintf(stderr,"malloc failed\n");
     exit(1);
   }

  int err,i;
  //创建线程
  for(i = 0; i < num; i++){
    th[i].index = i;
    th[i].thr_num = num;
    err = pthread_create(tid_arr+i,NULL,get_prime,th+i);
    //线程创建失败打印错误信息
    if(err != 0){
      fprintf(stderr,"pthread_create():%s\n",strerror(err));
      exit(1);
    }
  }
  //使用pthread_join 回收线程资源
  for(i = 0; i < num; i++){
    err = pthread_join(tid_arr[i],&tret);
    if(err != 0){
      fprintf(stderr,"pthread_join():%s\n",strerror(err));
      exit(1);
    }
  }
  free(th);
  free(tid_arr);
  printf("cleanup success\n");
  exit(0);
}
