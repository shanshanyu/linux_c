#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <syslog.h>
#include "proto.h"

#include <sys/ipc.h>
#include <sys/msg.h>
#define BUFSIZE 1024

/*
 * 创建一个守护进程：
 *创建消息队列
 *对 SIGINT 信号捕捉
 *从消息队列获取消息，判断是否是一个文件，是否有权限，如果都通过就给消息队列发送文件里面的数据
 *
 * 怎么判断文件已经发送完成？
 */
static int msgid;


void cleanup(){
  msgctl(msgid,IPC_RMID,NULL);
}

int main(){
  pid_t pid,sid;
  struct rlimit l;
  int i;
  int fd0,fd1,fd2;
  
  int fd;
  int n_read;
  char buf[BUFSIZE];

  key_t key;

  struct mymesg msg;

  pid = fork();
  if(pid < 0){
    perror("fork()");
    exit(1);
  }else if(pid == 0){
    //create session
    sid = setsid();
    if(sid < 0){
      perror("setsid()");
      exit(1);
    }
  }else{
    exit(0);
  }

  //call fork twice  
  pid = fork();
  if(pid < 0){
    perror("fork()");
    exit(1);
  }
  if(pid != 0)
    exit(0);

  

  //child execute next
  //set umask
  umask(0);

  //change current dir
  if(chdir("/") < 0){
    perror("chdir()");
    exit(1);
  }

  //close fd
  if(getrlimit(RLIMIT_NOFILE,&l) < 0){
    perror("getrlimit()");
    exit(1);
  }
  if(l.rlim_max == RLIM_INFINITY)
    l.rlim_max = 1024;

  for(i = 0; i < l.rlim_max; i++)
    close(i);

  //0 1 2redirect
  fd0 = open("/dev/null",O_RDWR);
  if(fd0 < 0){
    perror("open()");
    exit(1);
  }
  fd1 = dup(fd0);
  fd2 = dup(fd0);

  //log
  openlog(NULL,LOG_CONS,LOG_DAEMON);
  if(fd0 != 0 || fd1 != 1 || fd2 != 2){
    syslog(LOG_ERR,"unexpected file descriptor %d %d %d",fd0,fd1,fd2);
    exit(1);
  }

  //daemon initail over
 

  //create msg queue 
  key = ftok(KEYPATH,KEYPROJ); 
  if(key < 0){
    perror("ftok()");
    exit(1);
  }
  
  msgid = msgget(key,IPC_CREAT|0644);
  if(msgid < 0){
    perror("msgget()");
    exit(1);
  }  
  
  msg.mtype = 1;
 
  //wait client
  while(1){
    if(msgrcv(msgid,&msg,sizeof(msg)-sizeof(long),0,0) < 0){
      perror("msgrcv()");
      exit(1);
    }   

    //检查读取到的是否是文件，以及是否有权限读文件
    if(access(msg.mtext,R_OK) < 0){
      perror("access()");
      exit(1);
    }
    fd = open(msg.mtext,O_RDONLY);
    if(fd < 0){
      perror("open()");
      exit(1);
    }
   
    //read file and send to msg queue
    while(1){
      n_read = read(fd,buf,BUFSIZE);
      if(n_read < 0){
        perror("read()");
        exit(1);
      }
      if(n_read == 0) break;
      strncpy(msg.mtext,buf,n_read);
    
      if(msgsnd(msgid,&msg,sizeof(msg)-sizeof(long),0) < 0){
        perror("msgsnd()");
        exit(1);
      }
    }
  }


  exit(0);
}
