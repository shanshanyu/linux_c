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
  
  signal(SIGINT,sig_handler);
  //which 选择block 和unblock 都可以,保存之前的信号屏蔽字
  sigprocmask(SIG_UNBLOCK,&set,&oset);
  //unblock 后的信号屏蔽字有必要保存吗？
  //
  for(j = 0; i < 100; j++){
  //UNBLOCK 可以保证在下面这段代码中信号是 unblock 的
  sigprocmask(SIG_BLOCK,&set,NULL);
  for(i = 0; i < 5; i++){
   write(STDOUT_FILENO,"*",1);
   //sleep(1);
   nanosleep(&t,NULL);
  }
  write(STDOUT_FILENO,"\n",1);
  sigprocmask(SIG_UNBLOCK,&set,NULL);
 }

 //恢复之前的信号屏蔽字
 sigprocmask(SIG_SETMASK,&oset,NULL);
  return 0;
}
