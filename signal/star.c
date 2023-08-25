#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define LINE_SIZE 5

/*每秒钟向标准输出打印一个 *，打印 5 个后换行，期间不响应 SIGINT 信号，换行的时候才可以响应
 * 注意恢复信号屏蔽字和信号处理程序
 *
 *
 */

void int_handler(int signo){
  //向标准输出输出 !
  write(STDOUT_FILENO,"!",1);
}


int main(){
  int i,j;
  sigset_t set,oset;
  sigemptyset(&set);
  sigaddset(&set,SIGINT);

  struct sigaction sa_info,sa_save;  
  sa_info.sa_handler = int_handler;
  sa_info.sa_mask = set;

  struct timespec tv;
  tv.tv_sec = 1;
  tv.tv_nsec = 0;
  

  //注册信号捕捉函数
  if(sigaction(SIGINT,&sa_info,&sa_save) < 0){
    perror("sigaction()");
    exit(1);
  }

  //保存信号屏蔽字
  sigprocmask(SIG_UNBLOCK,&set,&oset);
  for(i = 0; i < LINE_SIZE; i++){
    //阻塞 sigint
    sigprocmask(SIG_BLOCK,&set,NULL);
    for(j = 0; j < LINE_SIZE; j++){
      write(STDOUT_FILENO,"*",1);
      nanosleep(&tv,NULL);
    }
    write(STDOUT_FILENO,"\n",1);
    //解除阻塞 signit
    sigprocmask(SIG_UNBLOCK,&set,NULL);
  }

  //恢复信号屏蔽字
  sigprocmask(SIG_SETMASK,&oset,NULL);
  sigaction(SIGINT,&sa_save,NULL);

  exit(0);
}
