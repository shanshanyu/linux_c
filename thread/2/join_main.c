#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//子线程
void *thr_func(void *arg){
  pthread_t tid = arg;
  void *tret;
  pthread_join(tid,&tret);
  printf("main thread exit valud:%d\n",tret);

}


//main 线程
int main(){
  pthread_t tid,cur_tid;
  cur_tid = pthread_self();
  tid = pthread_create(&tid,NULL,thr_func,cur_tid);
  sleep(1);
  pthread_exit(2);
}
