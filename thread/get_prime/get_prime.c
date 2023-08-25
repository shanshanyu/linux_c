#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM (RIGHT-LEFT+1)
/*
 *创建 n 个线程找 n 个质数（存在资源限制问题）
 */

struct thr_info{
  int index;
};

//有多少个数就创建多少个线程，是分块还是分片？
void *thr_getprimer(void *arg){
  int j;
  int i;
  int flag = 1;
  struct thr_info *th = arg;
  i = th->index;
  for(j = 2; j < i/2; j++)
    if(i % j == 0){
      flag = 0;
      break;
    }
  
  if(flag) printf("%d is primer\n",i);
  pthread_exit(arg);
}


int main(int argc,const char *argv[]){
  int i;
  int err;
  //线程id数组
  pthread_t tid_arr[THRNUM];

  void *tret;
  for(i = LEFT; i <= RIGHT; i++){
    //指针变量的初始化，如果不初始化指针变量直接取值会出现段错误
    struct thr_info *th = malloc(sizeof(struct thr_info));
    if(th == NULL){
      fprintf(stderr,"malloc err\n");
      exit(1);
    }
    th->index = i;
    //创建 THRNUM 个线程，每个线程负责一个数的判断
    err = pthread_create(tid_arr+i-LEFT,NULL,thr_getprimer,th);
    if(err != 0){
      fprintf(stderr,"pthread_create() failed:%s\n",strerror(err));
      exit(1);
    }
  }
   //THRNUM 个线程的资源回收
 for(i = LEFT; i <= RIGHT; i++){
    err = pthread_join(tid_arr[i-LEFT],&tret);
    if(err != 0){
      fprintf(stderr,"pthread_join() failed:%s\n",strerror(err));
      exit(1);
    }
    free(tret);
  }
  
  exit(0);
}

/*
 *当前程序还存在的问题：
 *1.资源类问题
 *2. 回收问题
 */
