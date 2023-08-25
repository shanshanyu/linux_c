#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
 *创建2个线程，然后回收，线程的清理程序验证是否执行 （线程从启动例程返回时不执行线程清理程序）
 *（线程被同一进程中的其他线程取消时，线程调用pthread_exit 时）都会执行线程清理程序）
 */

//清理程序
void clean(void *content){
  //content 传递一个参数  比如 thread1  thread2
  printf("%s clean ruuning\n");

}



//thread1,从启动例程返回
void *thr_fn1(void *arg){
 printf("thread%d running",int(arg));
 pthread_cleanup_push(clean,"thread1");   

}

//thread2，调用pthread_exit
void *thr_fn2(void *arg){


}


int main(){
  pthread_t tid1,tid2;
  int err;
  void *tret;
  err = pthread_create(&tid,NULL,thr_fn1,(void *)1);
  if(err != 0){
    strerror(err);
    exit(1);
  }  


  exit(0);
}
