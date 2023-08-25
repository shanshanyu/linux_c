#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//打印进程的退出状态,如果进程被信号 kill，显示是哪个信号


void pr_exit(int status){
  if(WIFEXITED(status))
    printf("program normal exit.\n");
  else if(WIFSIGNALED(status))
    printf("program abnoally exit,receive signal:%d\n",WTERMSIG(status));
  else if(WIFSTOPPED(status))
    printf("program stopped,receive signal:%d\n",WSTOPSIG(status));
}


int main(){
  pid_t pid;
  int status;
  pid = fork();
  if(pid < 0){
    perror("fork()");
  }else if(pid == 0){
    sleep(200);
    ;
    exit(0);
  }
  //回收子进程并获取子进程的终止状态
  wait(&status);
  pr_exit(status);
  exit(0);
}
