#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define THRNUM 3
#define LEFT 30000000
#define RIGHT 30000200


static int status = 0;
static int num;


static void *get_prime(void *p){
  int flag;
  int i;
  int tmp;
  while(1){
  if(status == 1){
    tmp = num;
    sleep(1);
    flag = 1;
    status = 0;
   // printf("change status to 0\n");
    for(i = 2; i < tmp/2; i++)
      if(tmp % i == 0){
        flag = 0;
        break;
      }
    if(flag) printf("%d is primer\n",tmp);
  }

  if(status == -1)
    pthread_exit(NULL);
  }
}


int main(){
  int i;
  pthread_t tid[THRNUM];
  pthread_mutex_t mut;
  pthread_mutex_init(&mut,NULL);
  //create 3 thread
  for(i = 0; i < THRNUM; i++)
    pthread_create(tid+i,NULL,get_prime,&mut);
  
  //input num
  for(i = LEFT; i <= RIGHT; i++){
    while(status != 0)
      sched_yield();

    status = 1;
 //   printf("change status to 1\n");
    num = i;
  //  printf("input %d success\n",num);
   // printf("status = %d\n",status);
  }
  
  status = -1;
  //reclain thread 
  for(i = 0; i < THRNUM; i++)
    pthread_join(tid[i],NULL);
 
  exit(0);
}
