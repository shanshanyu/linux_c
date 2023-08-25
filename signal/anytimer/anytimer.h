#ifndef ANYTIME_H__
#define ANYTIME_H__
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

typedef void (*func)(void *);

typedef struct timer{
  int sec;
  func f; //定时时间到后执行该函数
  void *arg;
  int pos;
}timer;

#define MAX_TIMER_SIZE 1024

timer *init_timer(int sec,func f,void *arg);
int destroy_timer(timer *t);
#endif
