#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include "my_msg.h"
static int flag = 1;
void handler(int signo){
  flag = 0;

}

int main(){
 
  key_t key;
  int msgid;
  struct mymsg_st msg_buf;
  int n_read;
  struct sigaction sa_info,sa_save;

  sigset_t set,oset;
  sigemptyset(&set);
  sigaddset(&set,SIGINT);

  sa_info.sa_mask = set;
  sa_info.sa_handler = handler;

  //设置信号捕捉函数
  sigaction(SIGINT,&sa_info,&sa_save);
  //设置信号屏蔽字
  sigprocmask(SIG_UNBLOCK,&set,&oset);  

  key = ftok(KEYPATH,KEYPROJ);
  if(key < 0){
    perror("ftok()");
    exit(1);
  } 

  msgid = msgget(key,IPC_CREAT|0600);   //创建
  if(msgid < 0){
    perror("msgget()");
    exit(1);
  }

  while(1){
    n_read = msgrcv(msgid,&msg_buf,sizeof(msg_buf)-sizeof(long),0,0);
    if(n_read < 0){
      perror("msgrcv()");
      msgctl(msgid,IPC_RMID,0);
      exit(1);
    }
    puts(msg_buf.mtext);
    if(!flag) break;
  }

  
  msgctl(msgid,IPC_RMID,0);   //销毁
  sigprocmask(SIG_SETMASK,&oset,NULL);
  sigaction(SIGINT,&sa_save,NULL);
  exit(0);
}
