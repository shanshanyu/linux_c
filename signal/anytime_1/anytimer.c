#include "anytimer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define MAX_TIMER_LEN 1024

//timer 数组
struct timer *timer_arr[MAX_TIMER_LEN];

static int inited = 1;

//信号处理程序
void sig_handler(int signo){
  int i;
  printf("handler running\n");
  for(i = 0; timer_arr[i]; i++){
    printf("%d\n",timer_arr[i]->time);
    if(--timer_arr[i]->time == 0){
       //时钟超时执行任务
       timer_arr[i]->task_func();
       //定时器重新赋值
       timer_arr[i]->time += timer_arr[i]->interval;
    }   
  }
  alarm(1);
}


timer *create_timer(void (*task_func)(void),int time){
  int i;
  timer *t = malloc(sizeof(timer));
  if(t  == NULL){
    fprintf(stderr,"malloc failed\n");
    exit(1);
  }

  t->task_func = task_func;
  t->interval = time;
  t->time = time;
  
  //如何在数组里面找到一个空位置？
  i = 0;
  while(timer_arr[i])
    i++;
  timer_arr[i] = t;

  if(signal(SIGALRM,sig_handler) == SIG_ERR){
    fprintf(stderr,"signal err\n");
    exit(1);
  }
  //程序第一次执行的时候设置闹钟
  if(inited == 1){
    inited = 0;
    alarm(1);
    printf("alarm set\n");
  }
 
  return t; 

}

int destroy_timer(timer *timer_st);

