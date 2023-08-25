
#ifndef ANYTIME_H
#define ANYTIME_H
typedef struct timer{
  void (*task_func)(void); //函数指针变量,函数没有返回值，没有参数
  void *arg; //函数参数
  int interval; //记录任务的循环时间
  int time; //记录当前剩余的秒数
}timer;

timer *create_timer(void (*task_func)(void),int time);

int destroy_timer(timer *timer_st);
#endif
