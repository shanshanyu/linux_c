#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
/*
 *从当前的system 函数执行情况来看：
 *未忽略 sigint,未阻塞 sigint
 *
 */

static void sig_chld(int sig){
  printf("catch sigchld\n");

}

static void sig_int(int sig){
  printf("catch sigint\n");

}

int system1(const char * cmdstring)
{
  pid_t pid;
  int status;
  if(cmdstring == NULL){
    return (1); //如果cmdstring为空，返回非零值，一般为1
  }
  if((pid = fork())<0){
    status = -1; //fork失败，返回-1
  }
  else if(pid == 0){
    execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
    _exit(127); // exec执行失败返回127，注意exec只在失败时才返回现在的进程，成功的话现在的
  }
  else{
  while(waitpid(pid, &status, 0) < 0){
    if(errno != EINTR){
    status = -1; //如果waitpid被信号中断，则返回-1
    break;
  }
  }
  }
  return status; //如果waitpid成功，则返回子进程的返回状态
}

int main(){
  int ret;
  //struct sigaction act,oact;
  //act.sa_handler = sig_handler;
  //sigemptyset(&act.sa_mask);
  //sigaddset(&act.sa_mask,SIGINT);
  //act.sa_flags = 0;
  //sigaction(SIGCHLD,&act,&oact);
  if(signal(SIGINT,sig_int) == SIG_ERR)
    printf("error\n");
  if(signal(SIGCHLD,sig_chld) == SIG_ERR)
    printf("error\n");
  ret = system1("ed");
  printf("ret = %d\n",ret);


  return 0;
}
