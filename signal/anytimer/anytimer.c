#include "anytimer.h"
#include <unistd.h>

static timer *timer_arr[MAX_TIMER_SIZE];

static int inited = 1;


void sig_handler(int arg){
  int i;
  for(i = 0; i< MAX_TIMER_SIZE; i++){
    if(timer_arr[i])
      if(--timer_arr[i]->sec  == 0)
        timer_arr[i]->f(timer_arr[i]->arg);
  }
  alarm(1);
}

//初始化一个定时器
timer *init_timer(int sec,func f,void *arg){
  int i;
  struct itimerval it;
  it.it_interval.tv_sec = 0;
  it.it_interval.tv_usec = 0;
  it.it_value.tv_sec = 1;
  it.it_value.tv_usec = 0;
  timer *t = malloc(sizeof(timer));
  if(t == NULL){
    fprintf(stderr,"malloc failed\n");
    return NULL;
  }

  t->sec = sec;
  t->f = f;
  t->arg = arg;

  for(i = 0; i < MAX_TIMER_SIZE; i++){
    if(!timer_arr[i]){
      timer_arr[i] = t; 
      t->pos = i;
      break;
    }
  }
  if(i == MAX_TIMER_SIZE){
    fprintf(stderr,"timer_arr is full\n");
    return NULL;
  }
  if(inited){
    inited = 0;
    //SIGARLM的行为先不保存
    signal(SIGALRM,sig_handler);
    //alarm(1);
    setitimer(ITIMER_REAL,&it,NULL);
  }
  return t; 
}

//销毁一个定时器
int destroy_timer(timer *t){
  timer_arr[t->pos] = NULL;
  free(t);
  return 0;
}
