#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <syslog.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int check_path(const char *path){
  if(access(path,F_OK) == -1){
    syslog(LOG_ERR,"access failed\n");
    return -1;
  }
  if(access(path,R_OK) == -1){
    syslog(LOG_ERR,"access failed\n");
    return -1;
  }
  return 0;
}

//推动状态机
void fsm_driver(fsm_st *fsm){
  switch(fsm->state){
    int r_ret;
    case STATE_R_PATH:
      //check path，从消息队列中读取 path，如果path不存在或者没有权限,重新读
      syslog(LOG_INFO,"read path");
      msgrcv(fsm->msgid,&fsm->path,PATHMAX,0,0);

      if(fsm->path.mtype == MSG_PATH){
        if(check_path(fsm->path.path) == -1)
          fsm->state = STATE_R_PATH;
        else
          fsm->state = STATE_S_DATA;
      }else
        fsm->state = STATE_R_PATH;
    break;
    
    case STATE_S_DATA:
      //如果文件未打开，打开文件
      if(!fsm->open_flag){
        fsm->fd = open(fsm->path.path,O_RDONLY);
        if(fsm->fd < 0){
          syslog(LOG_ERR,"open failed\n");
          fsm->state = STATE_R_PATH;
        }
        else{
          fsm->open_flag = 1;
          syslog(LOG_INFO,"open success\n");
        }
        
      }

      r_ret = read(fsm->fd,&fsm->u_data.data.data,DATAMAX);
      syslog(LOG_INFO,"r_ret = %d\n",r_ret);
      if(r_ret < 0){
        syslog(LOG_ERR,"read failed\n");
        fsm->state = STATE_R_PATH;
      }else if(r_ret == 0){
        fsm->u_data.eot.mtype = MSG_EOT;
        msgsnd(fsm->msgid,&fsm->u_data.eot,DATAMAX,0);
        fsm->open_flag = 0;
        sleep(5);
        fsm->state = STATE_R_PATH;
      }else{
        //send data
        fsm->u_data.data.mtype = MSG_DATA;
        fsm->u_data.data.datalen = r_ret;
        msgsnd(fsm->msgid,&fsm->u_data.data,fsm->u_data.data.datalen,0);
      
        fsm->state = STATE_S_DATA;
          
      }
    break;

  }
}

int main(int argc,const char *argv[]){
  //创建守护进程
  struct fsm_st fsm;
  int fd0,fd1,fd2;
  int i;
  struct rlimit l;
  pid_t pid;

  key_t key;
  int msgid;

  pid = fork();
  if(pid < 0){
    perror("fork()");
    exit(1);
  }else if(pid == 0){
    if(setsid() == -1){
      perror("setsid()");
      exit(1);
    }
  }else{
    exit(0);
  }

  pid = fork();
  if(pid < 0){
    perror("fork()");
    exit(1);
  }
  if(pid != 0) //父进程退出
    exit(0);

  //两次调用 fork 后的子进程，当前进程不是会话首进程了，不会获得终止终端了
  umask(0);
  if(chdir("/") == -1){
    perror("chdir()");
    exit(1);
  }
  
  //关闭不必要的 fd
  if(getrlimit(RLIMIT_NOFILE,&l) == -1){
    perror("getrlimit()");
    exit(1);
  }

  if(l.rlim_max == RLIM_INFINITY)
    l.rlim_max = 1024;
  for(i = 0; i < l.rlim_max; i++)
    close(i);

  fd0 = open("/dev/null",O_RDWR);
  if(fd0 < 0){
    perror("open()");
    exit(1);
  } 
  fd1 = dup(fd0);
  fd2 = dup(fd0);
  
  openlog(argv[0],LOG_CONS,LOG_DAEMON);
  if(fd0 != 0 || fd1 != 1 || fd2 != 2){
    syslog(LOG_ERR,"unexpected file descriptor %d %d %d",fd0,fd1,fd2);
    exit(1);
  }
  //daemon initial over
  
  key = ftok(KEYPATH,KEYPROJ);
  if(key < 0){
    syslog(LOG_ERR,"ftok() failed");
    exit(1);
  }
  
  msgid = msgget(key,IPC_CREAT|IPC_EXCL|0644);
  if(msgid < 0){
    syslog(LOG_ERR,"msgget failed\n");
  }
  fsm.msgid = msgid;
  fsm.state = STATE_R_PATH;   
  fsm.open_flag = 0;

  while(1)
    fsm_driver(&fsm);

  exit(0);
}
