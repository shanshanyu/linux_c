#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define BUFSIZE 1024
#define THRNUM 20
#define PATH "/tmp/out"


/*
 *创建20个线程，读取 /tmp/out 文件，每次打开加 1 然后写回 /tmp/out
 *
 */


void *thr_add(void *p){
  //每个线程的工作是 打开文件、 加1 、 写回文件
  FILE *fp;
  char buf[BUFSIZE];
  int tmp;
  int err;

  err = pthread_mutex_lock(p);
  if(err != 0){
    fprintf(stderr,"pthread_mutex_lock():%s\n",strerror(err));
    exit(1);
  }
  //打开文件
  fp = fopen(PATH,"r+");
  if(fp == NULL){
    fprintf(stderr,"fopen() error\n");
    exit(1);
  }
  
  if(fgets(buf,BUFSIZE,fp)  == NULL){
    fprintf(stderr,"fgets() error\n");
    exit(1);
  }

  tmp = atoi(buf)+1;

  fseek(fp,0,SEEK_SET);
  fprintf(fp,"%d",tmp);
  //关闭文件
  fclose(fp);
  err = pthread_mutex_unlock(p);
  if(err != 0){
    fprintf(stderr,"pthread_mutex_lock():%s\n",strerror(err));
    exit(1);
  }
  pthread_exit(NULL);
}


int main(){
  int i;
  int err;
  pthread_t tid_arr[THRNUM];
  
  pthread_mutex_t mutex;

  //初始化互斥量
  err = pthread_mutex_init(&mutex,NULL);
  //为什么下面这种初始化方法会失败？
  //mutex = PTHREAD_MUTEX_INITIALIZER;
  if(err != 0){
    fprintf(stderr,"pthread_mutex_init() :%s\n",strerror(err));
    exit(1);
  }

  //创建 THRNUM 个线程
  for(i = 0; i < THRNUM; i++){
    err = pthread_create(tid_arr+i,NULL,thr_add,&mutex);
    if(err != 0){
      fprintf(stderr,"pthread_create() failed:%s\n",strerror(err));
      exit(1);
    }
  }
  //回收线程资源
  for(i = 0; i < THRNUM; i++){
    err = pthread_join(tid_arr[i],NULL);
    if(err != 0){
      fprintf(stderr,"pthread_join() failed:%s\n",strerror(err));
      exit(1);
    }
  }

  err = pthread_mutex_destroy(&mutex);
  if(err != 0){
    fprintf(stderr,"pthread_mutex_destroy() failed:%s\n",strerror(err));
    exit(1);
  }
  exit(0);
}
