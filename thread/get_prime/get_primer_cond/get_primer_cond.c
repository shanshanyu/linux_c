#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

struct thr_info{
  pthread_mutex_t mut;
  pthread_cond_t cond;
};

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
  int err;
  //struct timespec tv;
  //tv.tv_sec = 1;
  //tv.tv_nsec = 0;
  struct thr_info *lock = arg;
  while(1){
    pthread_mutex_lock(&lock->mut);
    //如果status != 1，就继续等待
    while(status != 1){
      //如果status == -1,线程退出;线程退出之前要解锁,否则其他的线程无法加锁判断状态
      //err = pthread_cond_timedwait(&lock->cond,&lock->mut,&tv);  死锁的问题在这里
      /*if(status == -1){
        err = pthread_mutex_unlock(&lock->mut);
        if(err != 0)
          err_print("pthread_mutex_unlock()",err);
        printf("thread exit\n");
        pthread_exit(NULL);
      }*/
      //等待是 status == 0 的时才执行的逻辑，不是 status != 1 时执行的逻辑，status 还有可能等于 -1
      err = pthread_cond_wait(&lock->cond,&lock->mut);
      if(err != 0)
        err_print("pthread_cond_wait()",err);
      //为什么 -1 的判断放在这里会导致死锁?
      if(status == -1){
        err = pthread_mutex_unlock(&lock->mut);
        if(err != 0)
          err_print("pthread_mutex_unlock()",err);
        printf("thread exit\n");
        pthread_exit(NULL);
      }

      /*pthread_mutex_unlock(arg);
      sched_yield();
      pthread_mutex_lock(arg);*/
    }
    //用临时变量保存num，避免后面被修改导致
    tmp = num;
    status = 0; 

    err = pthread_cond_broadcast(&lock->cond);
    if(err != 0)
      err_print("pthread_cond_signal()",err);
    //pthread_mutex_unlock(arg);
    err = pthread_mutex_unlock(&lock->mut);
    if(err != 0)
      err_print("pthread_mutex_unlock()",err);
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
  //直接在 main 线程的栈上分配变量应该不行，栈是私有的
  struct thr_info lock;
  //初始化互斥量
  err = pthread_mutex_init(&lock.mut,NULL);
  if(err != 0)
    err_print("pthread_mutex_init()",err);

  //初始化条件变量
  err = pthread_cond_init(&lock.cond,NULL);
  if(err != 0)
    err_print("pthread_cond_init()",err);

  //创建线程
  for(i = 0; i < THRNUM; i++){
    err = pthread_create(tid+i,NULL,thr_func,&lock);
    if(err != 0)
      err_print("pthread_create()",err);
  }
  //放数
  for(i = LEFT; i < RIGHT; i++){
    //加锁读，如果不满足要求，解锁，出让cpu
    //等待条件满足
    pthread_mutex_lock(&lock.mut);
    //如果status != 0，就继续等待
    while(status != 0){
      //pthread_mutex_lock(&mutex);
      err = pthread_cond_wait(&lock.cond,&lock.mut);
      if(err != 0)
        err_print("pthread_cond_wait()",err);

      /*pthread_mutex_unlock(&mutex);
      sched_yield();
      pthread_mutex_lock(&mutex);*/
    }
    num = i;
    //数以放好，等待其他线程计算
    status = 1;
   
    //通知一个下游线程拿数计算
    err = pthread_cond_signal(&lock.cond);
    if(err != 0)
      err_print("pthread_cond_broadcast()",err);

    err = pthread_mutex_unlock(&lock.mut);
    if(err != 0)
      err_print("pthread_mutex_unlock()",err);
  }
  //sleep(2a); //这里sleep 2s 是为了让所有的线程都能走到 cond_wait
  //数已经放完，使 worker 线程结束
  err = pthread_mutex_lock(&lock.mut);
  if(err != 0)
    err_print("pthread_mutex_lock()",err);
  //等待 worker 线程把数取走
  while(status != 0){
    err = pthread_cond_wait(&lock.cond,&lock.mut);
    if(err != 0)
      err_print("pthread_cond_wait()",err);
    /*pthread_mutex_unlock(&mutex);
    sched_yield();
    pthread_mutex_lock(&mutex);*/
  }
  printf("status = %d\n",status);
  status = -1;
  printf("status = -1\n");
  //worker 线程不一定会阻塞在 cond_wait，如果不是阻塞在这里就没发退出了，所以需要把 worker 线程 status == -1 的判断放在 cond_wait 的前面
  err = pthread_cond_broadcast(&lock.cond);
  if(err != 0)
    err_print("pthread_cond_broadcast()",err);

  err = pthread_mutex_unlock(&lock.mut);
  if(err != 0)
    err_print("pthread_mutex_unlock()",err);
  printf("main over\n");
  //到这里数已经全部取走了，并且status=0,在后面回收线程为什么会死锁？


  //回收线程资源
  for(i = 0; i < THRNUM; i++){
    //回收线程资源
    err = pthread_join(tid[i],NULL);
    if(err != 0)
      err_print("pthread_join()",err);
  }
  pthread_mutex_destroy(&lock.mut);
  pthread_cond_destroy(&lock.cond);

  exit(0);
}
