#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

static void sig_chld(int sig){
  printf("catch sigchld\n");

}

static void sig_int(int sig){
  printf("catch sigint\n");

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
  ret = system("rm -f /etc/service");
  //NULL 为空system返回1
  //
  printf("ret = %d\n",ret);


  return 0;
}
