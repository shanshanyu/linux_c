#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <fcntl.h>
#include <syslog.h>
#include "proto.h"

//server 的状态机：初始化->读path->发data  每种状态的异常退出，正常退出

enum SERVER_STATE{
  STATE_R_PATH, //读PATH
  STATE_S_DATA, //发DATA
  STATE_Ex, //异常退出
  STATE_T //正常结束
};

typedef struct ftp_fsm_st{
  enum SERVER_STATE state;
  int fd; //收到path包中对应path的文件描述符 
  msg_path_t p_path; //path包
  msg_data_t p_data; //data包
  msg_eot_t p_eot;  //eot包
  int msgid; //消息队列标识符
  int read_count;
}ftp_fsm_t;

void daemonize(){
  pid_t pid;
  struct rlimit limit;
  int i;
  int fd0,fd1,fd2;

  //创建子进程,两次调用 fork 
  pid = fork();
  if(pid < 0){
    perror("fork()");
    exit(1);
  }else if(pid > 0)
    exit(0);

  //创建会话，脱离控制终端
  setsid();

  //设置文件模式创建屏蔽字,设置为0，保证守护进程有最大权限
  umask(0);
  //更改进程工作目录
  if(chdir("/") < 0){
    perror("chdir()");
    exit(1);
  }

  //关闭文件描述符
  if(getrlimit(RLIMIT_NOFILE,&limit) < 0){
    perror("getrlimit()");
    exit(1);
  }

  if(limit.rlim_max == RLIM_INFINITY)
    limit.rlim_max = 1024;

  for(i = 0; i < limit.rlim_max; i++)
    close(i);

  //标准输出、标准输出、标准错误重定向到 /dev/null
  fd0 = open("/dev/null",O_RDWR);
  fd1 = dup(fd0);
  fd2 = dup(fd0);  

  openlog("myftp_msg_v1",LOG_CONS,LOG_USER);
  if(fd0 != 0 && fd1 != 1 && fd2 != 2){
    syslog(LOG_ERR,"stdin stdout stderr redirect failed\n");
    exit(1);
  }
  
  //守护进程初始化完成
  syslog(LOG_INFO,"daemon initialize sucess\n");
}

int file_check(const char *path){
  //判断 path 是否是文件以及是否有读取权限
  int fd;
  struct stat st;

  if(stat(path,&st) < 0){
    syslog(LOG_ERR,"%s file check failed\n",path);
    return -1;
  }
  
  //判断传递的path是否是文件
  if(!S_ISREG(st.st_mode))
    syslog(LOG_ERR,"%s file check failed\n",path);

  //判断传递的path是否有权限打开
  fd = open(path,O_RDONLY);
  if(fd < 0){
    syslog(LOG_ERR,"%s file check failed",path);
    return -1;
  }
  return fd;
}

void ftp_fsm_driver(ftp_fsm_t *fsm){
  switch(fsm->state){
    case STATE_R_PATH:
      //收path
      if(msgrcv(fsm->msgid,&fsm->p_path,sizeof(fsm->p_path)-sizeof(long),MSG_PATH,0) < 0){
        fsm->state = STATE_Ex;
        break;
      }
      //path检查,如果文件检查失败，那么状态机的状态应该还是继续收path
      fsm->fd = file_check(fsm->p_path.path);
      if(fsm->fd < 0){
        syslog(LOG_ERR,"%s check failed,please check it\n",fsm->p_path.path);
        fsm->state = STATE_R_PATH;
        break;
      }
      //通过检查后send data
      fsm->state = STATE_S_DATA;
      break;
    case STATE_S_DATA:
      //每次读数前初始化缓冲区
      memset(&fsm->p_data,'\0',sizeof(fsm->p_data));
       
      //fsm->p_data.mtype = MSG_DATA;  这句可以不要，包的类型在 if..else if 中会指定
      fsm->read_count = read(fsm->fd,fsm->p_data.data,DATA_MAX);
      if(fsm->read_count < 0)
        fsm->state = STATE_Ex;
      else if(fsm->read_count == 0){
        //数据传输完后发送eot包
        fsm->p_eot.mtype = MSG_EOT;
        if(msgsnd(fsm->msgid,&fsm->p_eot,0,0) < 0){
          fsm->state = STATE_Ex;
          break;
        }
        syslog(LOG_INFO,"%s file send over\n",fsm->p_path.path);
        fsm->state = STATE_R_PATH; 
      }
      else{
        fsm->p_data.mtype = MSG_DATA;  
        //设置包的实际长度
        fsm->p_data.len = fsm->read_count;
        if(msgsnd(fsm->msgid,&fsm->p_data,sizeof(fsm->p_data)-sizeof(long),0) < 0){
          fsm->state = STATE_Ex;
          break;
        }
        fsm->state = STATE_S_DATA;
      }
      break;
    case STATE_Ex:
      break;
    case STATE_T:
      break;
    default:
      syslog(LOG_ERR,"unrecognized fsm state\n");
      fsm->state = STATE_Ex;
  }
}

//创建消息队列
int create_msgq(ftp_fsm_t *fsm){
  key_t key = ftok(KEY_PATH,KEY_PROJ);
  if(key < 0){
    syslog(LOG_ERR,"ftok() failed\n");
    return -1;
  }
  fsm->msgid = msgget(key,IPC_CREAT|IPC_EXCL|0644);
  if(fsm->msgid < 0){
    syslog(LOG_ERR,"msgget() failed\n");
    return -1;
  }
  return 0;
}

int main(){
  //状态机结构
  ftp_fsm_t fsm;

  daemonize(); //将进程初始化成守护进程
  
  if(create_msgq(&fsm) < 0){
    syslog(LOG_ERR,"create msg queue failed\n");
    exit(1);
  }

  //server状态机初始状态为 收path
  fsm.state = STATE_R_PATH;


  //推状态机 
  while(fsm.state != STATE_T && fsm.state != STATE_Ex)
    ftp_fsm_driver(&fsm);

  if(fsm.state == STATE_T)
    syslog(LOG_ERR,"process exit nornally");
  else if(fsm.state == STATE_Ex)
    syslog(LOG_ERR,"process exit abnornally");

  //删除消息队列
  msgctl(fsm.msgid,IPC_RMID,NULL);

  exit(0);
}
