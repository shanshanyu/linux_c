#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "my_msg.h"



int main(){
  key_t key;
  int msgid;
  struct mymsg_st msg_buf;
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
  
  msg_buf.mtype  = 1;
  strcpy(msg_buf.mtext,"Tom");

  if(msgsnd(msgid,&msg_buf,sizeof(msg_buf)-sizeof(long),0) < 0){
    perror("msgsnd()");
    exit(1);
  }

  fprintf(stdout,"send over\n");
  exit(0);
}
