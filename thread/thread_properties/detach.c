#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

//以分离状态创建线程

void *thr_func(void *arg){
  printf("i am new thread\n");
  return NULL;
}


int main(){
  pthread_t tid;
  pthread_attr_t tattr;
  int err;
  //初始化线程属性变量
  err = pthread_attr_init(&tattr);
  if(err != 0){
    fprintf(stderr,"init attr failed,because:%s\n",strerror(err));
    exit(1);
  }
  //设置线程分离属性
  err = pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);
  if(err != 0){
    fprintf(stderr,"set attr failed,because:%s\n",strerror(err));
    exit(1);
  }
  //创建线程
  err = pthread_create(&tid,&tattr,thr_func,NULL);
  if(err != 0){
    fprintf(stderr,"create thread failed,because:%s\n",strerror(err));
    exit(1);
  }
  pthread_attr_destroy(&tattr);
  return(err);
  //不能join，因为线程已经设置了分离属性
 /* err = pthread_join(tid,NULL);
  if(err != 0){
    fprintf(stderr,"join failed,because:%s\n",strerror(err));
    exit(1);
  }
*/
}
