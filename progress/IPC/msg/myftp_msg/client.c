#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

void fsm_driver(fsm_st *fsm){
  switch(fsm->state){
    case STATE_S_PATH:
      fsm->path.mtype = MSG_PATH;
      msgsnd(fsm->msgid,&fsm->path,PATHMAX,0);
      printf("send over\n");
      fsm->state = STATE_R_DATA;
      printf("send path\n");
      sleep(2);
    break;

    case STATE_R_DATA:
      printf("receive data\n");
      msgrcv(fsm->msgid,&fsm->u_data,DATAMAX,0,0);
      printf("mtype = %d\n",fsm->u_data.mtype);
      if(fsm->u_data.mtype == MSG_DATA){
        write(fsm->fd,fsm->u_data.data.data,fsm->u_data.data.datalen);
        fsm->state = STATE_R_DATA;
      }else if(fsm->u_data.mtype == MSG_EOT)
        fsm->state = STATE_T;
      else
        fsm->state = STATE_Ex;
    break;

    case STATE_Ex:
      fsm->state = STATE_T;
    break;

    case STATE_T:
    break;
  }


}



int main(int argc,const char *argv[]){
  key_t key;
  int msgid;
  fsm_st fsm;

  if(argc != 2){
    fprintf(stderr,"Usage:%s filename\n",argv[0]);
    exit(1);
  }
  
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

  fsm.msgid = msgid;
  fsm.state = STATE_S_PATH;
  strncpy(fsm.path.path,argv[1],PATHMAX);
  printf("init over\n");

  fsm.fd = open("/tmp/out",O_WRONLY|O_CREAT|O_TRUNC,0644);
  if(fsm.fd < 0){
    perror("open()");
    exit(1);
  }
  //推状态机
  while(fsm.state != STATE_T)
    fsm_driver(&fsm);  

  exit(0);
}
