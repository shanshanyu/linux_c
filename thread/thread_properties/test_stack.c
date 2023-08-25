#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

/*
 *测试线程栈默认大小
 *
 */

int main(){
  pthread_attr_t attr;
  int err;
  size_t size;
  err = pthread_attr_init(&attr);
  if(err != 0){
    fprintf(stderr,"init pthread_attr_t failed:%s\n",strerror(err));
    exit(1);
  }
 
  //能否改变进程的栈空间大小的限制
  err = pthread_attr_setstacksize(&attr,1*513*519);
  printf("err = %d\n",err);
  if(err != 0){
    fprintf(stderr,"pthread_attr_setstacksize() failed:%s\n",strerror(err));
    exit(1);
  }

  err = pthread_attr_getstacksize(&attr,&size);
  if(err != 0){
    fprintf(stderr,"getstacksize failed:%s\n",strerror(err));
    exit(1);
  }
  //pthread_attr_getstacksize 的 stacksize 是什么意思？
  //代表的进程的栈大小限制：8M
  printf("stack size:%d\n",size);  
  
  
  //打印线程栈大小
  printf("thread stack default size:%d\n",sysconf(_SC_THREAD_ATTR_STACKSIZE));
  printf("thread stack addr:%p\n",sysconf(_SC_THREAD_ATTR_STACKADDR));
  printf("线程栈可用的最小空间:%d\n",sysconf(_SC_THREAD_STACK_MIN)); //16k
  pthread_attr_destroy(&attr);
  return 0;
}
