#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define THRNUM 4

/*
 *交叉打印 abcd 怎么实现？
 * 创建 4 个线程，分别打印 a b c d    线程要控制先后顺序，a 线程执行完再执行 b
 * 前一个线程对后一个线程解锁，用这种方法来实现顺序打印  链式解锁
 */

struct thr_info{
  int c;
  int index;
};

static pthread_mutex_t mutex_arr[THRNUM];

static void *print_str(void *arg){
  struct thr_info *th = arg;
  int err;
  int count = 0;
  while(1){
    //互斥量加锁
    err =  pthread_mutex_lock(mutex_arr+th->index);
    if(err != 0){
      fprintf(stderr,"pthread_mutex_init():%s\n",strerror(err));
      exit(1);
    }
    printf("%c",th->c);
    //对后一个互斥量解锁
    err = pthread_mutex_unlock(mutex_arr+((th->index+1)%THRNUM));
    if(err != 0){
      fprintf(stderr,"pthread_mutex_unlock():%s\n",strerror(err));
      exit(1);
    }
    count++;
    if(count == 10)
      break;
  }
  pthread_exit(arg);
}


int main(){
  pthread_t tid[THRNUM];
  int err;
  int i;
  
  struct thr_info *th;
  void *tret;
  //创建线程
  for(i = 0; i < THRNUM; i++){
    //每个线程创建一块，便于pthread_exit返回时资源回收
    th = malloc(sizeof(struct thr_info));
    if(th == NULL){
      fprintf(stderr,"malloc failed\n");
      exit(1);
    }
    //初始化thr_info
    th->c = 'a'+i;
    th->index = i;
    //互斥量初始化
    if(pthread_mutex_init(mutex_arr+i,NULL) != 0){
      fprintf(stderr,"pthread_mutex_init():%s\n",strerror(err));
      exit(1);
    }
    //加锁
    if(pthread_mutex_lock(mutex_arr+i) != 0){
      fprintf(stderr,"pthread_mutex_lock():%s\n",strerror(err));
      exit(1);
    }
    //创建线程
    err = pthread_create(tid+i,NULL,print_str,th);
    if(err){
      fprintf(stderr,"pthread_create():%s\n",strerror(err));
      exit(1);
    }
  }
  //对一个互斥量解锁，让第一个线程先运行
  if(pthread_mutex_unlock(mutex_arr) != 0){
    fprintf(stderr,"pthread_unlock():%s\n",strerror(err));
    exit(1);
  }

  //线程资源回收
  for(i = 0; i < THRNUM; i++){
    err = pthread_join(tid[i],&tret);
    if(err){
      fprintf(stderr,"pthread_join():%s\n",strerror(err));
      exit(1);
    }
    free(tret);
  }

  exit(0);
}
