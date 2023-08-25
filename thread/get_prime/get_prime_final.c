#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define LEFT 30000000
#define RIGHT 30000200
//使用 3 个线程
#define THRNUM 3

//这个程序的确定是多个线程加锁，存在忙等

static volatile int status = 0;
static int num;
/*
 * status 有 3 种状态：
 * 0： 等待放数
 * 1： main线程已经放好数了，等待worker线程去计算
 * -1：数已经全部放完，等待 worker 线程退出
 *
 */

/*
 *线程池的标准写法 ：计算质数
 *
 * 用 status 作为状态变量
 */

//打印质数
static void get_primer(int num){
  if(num == 1 || num == 2)
    printf("%d is primer\n",num);
  int i;
  int flag = 1;
  for(i = 2; i < num/2; i++)
    if(num % i == 0){
      flag = 0;
      break;
    }
  
  if(flag) printf("%d is primer\n",num);
}


//打印错误日志
static void err_print(const char *s,const int err){
   fprintf(stderr,"%s err:%s\n",s,strerror(err));
   exit(1);
}


//worker线程的作用是：
//拿数计算
void *thr_func(void *arg){
  int tmp;
  while(1){
    pthread_mutex_lock(arg);
    //如果status != 1，就继续等待
    while(status != 1){
      //如果status == -1,线程退出;线程退出之前要解锁,否则其他的线程无法加锁判断状态
      if(status == -1){
        pthread_mutex_unlock(arg);
        pthread_exit(NULL);
      }
      pthread_mutex_unlock(arg);
      sched_yield();
      pthread_mutex_lock(arg);
    }
    //用临时变量保存num，避免后面被修改导致
    tmp = num;
    status = 0; 
    pthread_mutex_unlock(arg);
    //功能
    get_primer(tmp);
  }
  
}


//main 线程的作用是：
//创建线程
//功能代码（放数）
//回收线程资源
int main(){
  pthread_t tid[THRNUM];
  int i,err;
  pthread_mutex_t mutex;
  //初始化互斥量
  err = pthread_mutex_init(&mutex,NULL);
  if(err != 0)
    err_print("pthread_mutex_init()",err);
  //创建线程
  for(i = 0; i < THRNUM; i++){
    err = pthread_create(tid+i,NULL,thr_func,&mutex);
    if(err != 0)
      err_print("pthread_create()",err);
  }
  //放数
  for(i = LEFT; i < RIGHT; i++){
    //加锁读，如果不满足要求，解锁，出让cpu
    //等待条件满足
    pthread_mutex_lock(&mutex);
    //如果status != 0，就继续等待
    while(status != 0){
      //pthread_mutex_lock(&mutex);
      pthread_mutex_unlock(&mutex);
      sched_yield();
      pthread_mutex_lock(&mutex);
    }
    num = i;
    //数以放好，等待其他线程计算
    status = 1;
    pthread_mutex_unlock(&mutex);
  }
  //数已经放完，使 worker 线程结束
  pthread_mutex_lock(&mutex);
  //等待 worker 线程把数取走
  while(status != 0){
    pthread_mutex_unlock(&mutex);
    sched_yield();
    pthread_mutex_lock(&mutex);
  }

  status = -1;
  pthread_mutex_unlock(&mutex);

  
  //回收线程资源
  for(i = 0; i < THRNUM; i++){
    err = pthread_join(tid[i],NULL);
    if(err != 0)
      err_print("pthread_join()",err);
  }

  exit(0);
}
