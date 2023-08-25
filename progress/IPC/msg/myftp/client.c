#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "proto.h"

/*
 *传递一个文件名到管道中，然后从管道中读取数据放到 /tmp 下的同名文件
 *
 */

int main(int argc,const char *argv[]){
  key_t key;
  int msgid;

  struct mymesg msg;

  int fd;
  int n_read;

  if(argc != 2){
    fprintf(stderr,"Usage:%s filename\n",argv[0]);
    exit(1);
  }

  //get msg queue
  key = ftok(KEYPATH,KEYPROJ);
  if(key < 0){
    perror("ftok()");
    exit(1);
  }

  msgid = msgget(key,0);
  if(msgid < 0){
    perror("msgget()");
    exit(1);
  }

 
  //test file
  if(access(argv[1],R_OK) < 0){
    perror("access()");
    exit(1);
  }

 
  fd = open("/tmp/out",O_WRONLY|O_CREAT|O_TRUNC,0644);
  if(fd < 0){
    perror("open()");
    exit(1);
  } 
  
  strcpy(msg.mtext,argv[1]); 
  msg.mtype = 1;

  if(msgsnd(msgid,&msg,sizeof(msg)-sizeof(long),0) < 0){
    perror("msgsnd()");
    exit(1);
  }
  //文件放好后什么时候开始读？
  sleep(1);



  while(1){
    if(msgrcv(msgid,&msg,sizeof(msg)-sizeof(long),0,0) < 0){
      perror("msgrcv()");
      exit(1);
    }

    n_read = strlen(msg.mtext);
    if(write(fd,msg.mtext,n_read) < 0){
      perror("write()");
      exit(1);
    }
    
    if(strchr(msg.mtext,EOF))
       break;   
  }  

  exit(0);
}
