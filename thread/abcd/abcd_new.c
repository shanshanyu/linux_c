#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define THRNUM 4

/*
 *创建 4 个线程，分别打印 a b c d，规律性的打印 abcd  用互斥量来实现
 *a解锁b，b 解锁c，c解锁d，d 解锁 a
 *
 */


static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int num = 0;

static void *print_char(void *p){
  int err;
  int n = *(int *)p;
  int c = 'a' + n;
  //获取锁,输出，释放下一个锁
  while(1){
    err = pthread_mutex_lock(&mut);
    if(err != 0){
      fprintf(stderr,"pthread_mutex_lock():%s\n",strerror(err));
      exit(1);
    }
    while(n != num)
      pthread_cond_wait(&cond,&mut);
   
    write(STDOUT_FILENO,&c,1);
    num = (num + 1) % THRNUM;
    err = pthread_cond_broadcast(&cond);
    if(err != 0){
      fprintf(stderr,"pthread_cond_broadcast():%s\n",strerror(err));
      exit(1);
    }
    err = pthread_mutex_unlock(&mut);
    if(err != 0){
      fprintf(stderr,"pthread_mutex_unlock():%s\n",strerror(err));
      exit(1);
    }
  }

  pthread_exit(NULL);
}

int main(){
  pthread_t tid[THRNUM];
  int err;
  int i;
  int index[THRNUM];
    //创建线程
  for(i = 0; i < THRNUM; i++){
    index[i] = i;
    err = pthread_create(tid+i,NULL,print_char,index+i);
    if(err != 0){
      fprintf(stderr,"pthread_create():%s\n",strerror(err));
      exit(1); 
   }
  }
  printf("create over\n");
  alarm(5);
  //回收线程
  for(i = 0; i < THRNUM; i++){
    err = pthread_join(tid[i],NULL);
    if(err != 0){
     fprintf(stderr,"pthread_join():%s\n",strerror(err));
     exit(1);
    }
  }   

 exit(0);
}
