#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/*
 * 信号驱动：第一行打完停止，收到信号再继续
 */

void sig_handler(int sig){
  write(STDOUT_FILENO,"!",1);
}

int main(){
  int i,j;
  sigset_t set,oset;
  //将信号集置为0
  sigemptyset(&set);
  //将 sigint 位置为1
  sigaddset(&set,SIGINT);
  
  signal(SIGINT,sig_handler);
  //which 选择block 和unblock 都可以,保存之前的信号屏蔽字
  sigprocmask(SIG_UNBLOCK,&set,&oset);
  //unblock 后的信号屏蔽字有必要保存吗？
  sigprocmask(SIG_BLOCK,&set,NULL);
  for(j = 0; i < 100; j++){
  //UNBLOCK 可以保证在下面这段代码中信号是 unblock 的
  for(i = 0; i < 5; i++){
   write(STDOUT_FILENO,"*",1);
   sleep(1);
  }
  write(STDOUT_FILENO,"\n",1);
  //信号驱动
  if(sigsuspend(&oset) != -1){
    perror("sissuspend");
    exit(1);
  }
  //这里如果非阻塞，信号能响应多次
 }

 //恢复之前的信号屏蔽字
 sigprocmask(SIG_SETMASK,&oset,NULL);
  return 0;
}
