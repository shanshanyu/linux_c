#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "proto.h"


//客户端状态机状态
enum CLIENT_STATE{
  STATE_S_PATH,
  STATE_R_DATA,
  STATE_Ex,
  STATE_T
};

typedef struct ftp_fsm_st{
  int msgid;
  enum CLIENT_STATE state;
  msg_path_t p_path;
  union s2c_st u_data;
  int dst_fd;//dst_filename
}ftp_fsm_t;

void ftp_fsm_driver(ftp_fsm_t *fsm){
  switch(fsm->state){
    case STATE_S_PATH:
      fsm->p_path.mtype = MSG_PATH;
      if(msgsnd(fsm->msgid,&fsm->p_path,sizeof(fsm->p_path)-sizeof(long),0) < 0){
        fsm->state = STATE_Ex;
        break;
      }
      fsm->state = STATE_R_DATA;
      break;
    case STATE_R_DATA:
      fprintf(stderr,"dst_fd=%d\n",fsm->dst_fd);
      if(msgrcv(fsm->msgid,&fsm->u_data,sizeof(fsm->u_data)-sizeof(long),-2,0) < 0){
        fprintf(stderr,"msgrcv() failed\n");
        fsm->state = STATE_Ex;
        break;
      }
      if(fsm->u_data.mtype == MSG_DATA){
        if(write(fsm->dst_fd,fsm->u_data.p_data.data,fsm->u_data.p_data.len) != fsm->u_data.p_data.len){
          fsm->state = STATE_Ex;
          break;
        }
        fsm->state = STATE_R_DATA;
      }else if(fsm->u_data.mtype == MSG_EOT){
        printf("over");
        fsm->state = STATE_T;
      }
      break;
    case STATE_Ex:
      break;
    case STATE_T:
      break;
    default:
      fprintf(stderr,"unrecognize fsm state\n");
      fsm->state = STATE_Ex;
  }
}

//加入消息队列
int create_msgq(ftp_fsm_t *fsm){
  key_t key = ftok(KEY_PATH,KEY_PROJ);
  if(key < 0){
    perror("ftok()");
    return -1;
  }

  fsm->msgid = msgget(key,0);
  if(fsm->msgid < 0){
    perror("msgget()");
    return -1;
  } 
  return 0;
}

//文件名通过参数 argv[1] 传递
int main(int argc,const char *argv[]){
  if(argc != 3){
    fprintf(stderr,"Usage:%s src_filename(absolute path) dst_filename\n",argv[0]);
    exit(1);
  }

  //定义状态机结构
  ftp_fsm_t fsm;

  //打开目标文件
  fsm.dst_fd = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0644);
  if(fsm.dst_fd < 0){
    perror("open()");
    exit(1);
  }

  //初始化状态结构
  if(create_msgq(&fsm) < 0){
    fprintf(stderr,"get msg queue id err\n");
    exit(1);
  }
  //初始化path
  strcpy(fsm.p_path.path,argv[1]);

  printf("dst_fd_yeah = %d\n",fsm.dst_fd);
  //设置状态机初始状态
  fsm.state = STATE_S_PATH;

  printf("union length = %ld\n",sizeof(fsm.u_data));  

  //推动状态机
  while(fsm.state != STATE_Ex && fsm.state != STATE_T)
    ftp_fsm_driver(&fsm);
  
  //判断程序的退出状态
  if(fsm.state == STATE_Ex)
    fprintf(stderr,"process exit abnornal\n");
  else if(fsm.state == STATE_T)
    fprintf(stderr,"process exit nornally\n");
 
   exit(0);
}
