#include <stdio.h>
#include <stdlib.h>


static void err_print(const char *func_name,int err_num){
  fprintf(stderr,"%s failed:%s\n",func_name,strerror(err_num));
  exit(1);
}

struct st_sem *my_sem_init(int sem_num){
  int err;
  if(sem_num < = 0)
    return NULL;
  struct st_sem *sem = malloc(sizeof(struct st_sem));
  if(sem == NULL){
    fprintf(stderr,"malloc failed\n");
    exit(1);
  }
 
  //初始化胡吃两  
  err = pthread_mutex_init(&sem->mut,NULL);
  if(err != 0)
    err_print("pthread_mutex_init()",err);
  
  //初始化条件变量
  err = pthread_cond_init(&sem->cond,NULL);
  if(err != 0)
    err_print("pthread_cond_init()",err);

  //初始化信号个数
  sem->sem_num = sem_num;

  return sem;
}

int my_sem_add(struct st_sem *sem,int num){
  int err;
  if(num <= 0)
    return -1;
  err = pthread_mutex_lock(&sem->mut);
  if(err != 0)
    err_print("pthread_mutex_lock()",err);
  
  sem->sem_num += num;
  //条件改变后发送通知
  err = pthread_cond_broadcast(&sem->cond);
  if(err != 0)
    err_print("pthread_cond_broadcast()",err);

  err = pthread_mutex_unlock(&sem->mut);
  if(err != 0)
    err_print("pthread_mutex_unlock()",err);
  return 0;
}
int my_sem_sub(struct st_sem *sem,int num){
  int err;
  if(num <= 0)
    return -1;
  err = pthread_mutex_lock(&sem->mut);
  if(err != 0)
    err_print("pthread_mutex_lock()",err);
 
  while(sem->num - num < 0)
    pthread_cond_wait(&sem->cond,&sem->mut);

  sem->sem_num -= num;
  err = pthread_mutex_unlock(&sem->mut);
  if(err != 0)
    err_print("pthread_mutex_unlock()",err);

  return 0;
}

int my_sem_destroy(struct st_sem *sem){
  int err;
  err = pthread_mutex_destroy(&sem->mut);
  err = pthread_cond_destroy(&sem->cond);
  sem->sem_num = 0;
  return 0;
}
