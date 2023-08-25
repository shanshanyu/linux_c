#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define LEFT 30000000
#define RIGHT 30000200
/*
 *创建 x 个线程来找质数，x 人为输入
 */

struct thr_info{
  int index;
  int thr_num;
};

//创建n个线程来计算m个数中的质数
void *thr_getprimer(void *arg){
  int i,j;
  int flag;
  struct thr_info *th = arg;
  for(i = LEFT+th->index; i <= RIGHT; i+=th->thr_num){
    flag = 1;
    for(j = 2; j < i/2; j++)
    if(i % j == 0){
      flag = 0;
      break;
    }
    if(flag) printf("%d is primer\n",i);
  }
  pthread_exit(arg);
}


int main(int argc,const char *argv[]){
  int i;
  int err;
 
  void *tret;
  int n;
  printf("input n thread to cal prime:");
  scanf("%d",&n);
 //线程id数组 
  pthread_t *tid_arr = malloc(sizeof(pthread_t)*n);
  if(tid_arr == NULL){
    fprintf(stderr,"malloc failed\n");
    exit(1);
  }
 
  for(i = 0; i < n; i++){
    //创建一个结构体用来传参
    struct thr_info *th = malloc(sizeof(struct thr_info));
    if(th == NULL){
      fprintf(stderr,"malloc err\n");
      exit(1);
    }
    th->index = i;
    th->thr_num = n;
    //创建 THRNUM 个线程，每个线程负责一个数的判断
    err = pthread_create(tid_arr+i,NULL,thr_getprimer,th);
    if(err != 0){
      fprintf(stderr,"pthread_create() failed:%s\n",strerror(err));
      exit(1);
    }
  }
   //THRNUM 个线程的资源回收
 for(i = 0; i < n; i++){
    err = pthread_join(tid_arr[i],&tret);
    if(err != 0){
      fprintf(stderr,"pthread_join() failed:%s\n",strerror(err));
      exit(1);
    }
    free(tret);
  }
  
  free(tid_arr);
  exit(0);
}

/*
 *当前程序还存在的问题：
 *1.
 *
 */
