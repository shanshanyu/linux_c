#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/*
 *测试信号集合sigprocmask 的关系
 *如果 sigempty 设置的信号屏蔽字会响应信号吗：
 *可以响应
 *如果 sigfullset 设置的信号屏蔽字会响应信号吗：
 *不会响应
 */


int main(void){
  sigset_t set,tmp_set;
  int err;
  if(sigfillset(&set)  == -1){
    fprintf(stderr,"sigfullset err\n");
    exit(1);
  }
  sigprocmask(SIG_SETMASK,&set,NULL);
  while(1){
    sleep(2);
    sigpending(&tmp_set);
    err = sigismember(&tmp_set,SIGINT);
    if(err == 1)
      printf("SIGINT in pending\n");
    else if(err == 0)
      printf("SIGINT not in pending\n");
    else
      printf("error\n");
  }

  exit(0);
}
