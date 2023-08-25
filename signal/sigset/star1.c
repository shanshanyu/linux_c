#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

/*
 *每秒打印一个 *，5s换行继续打印
 *
 *作为一个模块的写法，sigpromask应该怎么调用？
 *不知道在进入这个模块前信号是否是 block 的？需要先 unblock
 *然后在禁止响应信号的代码前 block，可以响应的时候 unblock   最后要恢复初始的信号屏蔽字  (标准写法，四句 sigprocmask）
 */

// 使用sigaction功能实现

void sig_handler(int sig){
  write(STDOUT_FILENO,"!",1);
}

int main(){
  int i,j;
  sigset_t set,oset;
  struct timespec t;
  t.tv_sec = 1;
  t.tv_nsec = 0;
  //将信号集置为0
  sigemptyset(&set);
  //将 sigint 位置为1
  sigaddset(&set,SIGINT);
  struct sigaction sig_info;
  sig_info.sa_handler = sig_handler;
  //在调用信号处理程序期间要阻塞哪些信号
  sig_info.sa_mask = set;
  
  //signal(SIGINT,sig_handler);
  //设置信号捕捉函数
  sigaction(SIGINT,&sig_info,NULL);
  //保留之前的信号屏蔽字
  sigprocmask(SIG_UNBLOCK,&set,&oset);
  for(j = 0; i < 100; j++){
  //阻塞,之前是否是阻塞不关心
  sigprocmask(SIG_BLOCK,&set,NULL);
  for(i = 0; i < 5; i++){
   write(STDOUT_FILENO,"*",1);
   //sleep(1);
   nanosleep(&t,NULL);
  }
  write(STDOUT_FILENO,"\n",1);
  //解除阻塞
  sigprocmask(SIG_UNBLOCK,&set,NULL);
 }

 //恢复之前的信号屏蔽字
 sigprocmask(SIG_SETMASK,&oset,NULL);
  return 0;
}
