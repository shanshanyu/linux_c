#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <sys/select.h>
#include "proto.h"

#define MAX_RETRY_TIMES 5

#define BUFSIZE 1024

//服务端状态机的几种状态
enum SERVER_STATE{
  STATE_R_PATH,
  STATE_S_DATA,
  STATE_Ex,
  STATE_T
};

//服务端状态机结构
typedef struct fsm_udp_st{
  int state; //状态机状态
  int sockfd; //socket文件描述符
  struct sockaddr_in laddr; //server 端地址
  struct sockaddr_in raddr; //client 端地址
  socklen_t raddrlen;
  union udp_s2c_st packet;
  int read_count;
  int fd; //path 文件的文件描述符
  long recv_ackno; //收到的 ackno
  long local_ackno; //本地的 ackno
  fd_set rfds;
  struct timeval tv;
  int count; //超时计数
  int ret;
}fsm_udp_t;


void daemonize(){
  pid_t pid;
  struct rlimit limit;
  int i;
  int fd0,fd1,fd2;

  //fork创建子进程
  pid = fork();
  if(pid < 0){
    perror("fork()");
    exit(1);
  }else if(pid > 0)
    exit(0);

  //创建会话
  if(setsid() < 0){
    perror("setsid()");
    exit(1);
  }

  //设置程序的文件模式创建屏蔽字  默认为 0002 设置为0，给与最大权限
  umask(0);

  //更改进程的工作目录
  if(chdir("/") < 0){
    perror("chdir()");
    exit(1);
  }

  //关闭不必要的文件描述符
  if(getrlimit(RLIMIT_NOFILE,&limit) < 0){
    perror("getrlimit()");
    exit(1);
  }

  if(limit.rlim_max == RLIM_INFINITY)
    limit.rlim_max = 1024;

  for(i = 0; i < limit.rlim_max; i++)
    close(i);


  fd0 = open("/dev/null",O_RDWR);
  fd1 = dup(fd0);
  fd2 = dup(fd0);

  //日志文件打印到系统目录
  openlog("myftp_msg_v1",LOG_CONS,LOG_USER);
  if(fd0 != 0 && fd1 != 1 && fd2 != 2){
    syslog(LOG_ERR,"stdin stdout stderr redirect failed\n");
    exit(1);
  }
  syslog(LOG_INFO,"daemon intial success.\n");
}

//文件检查
int file_check(const char *path){

  int fd;
  struct stat st;
  
  if(stat(path,&st) < 0){
    syslog(LOG_ERR,"%s file check failed\n",path);
    return -1;
  }

  //判断是否是一个REG文件
  if(!S_ISREG(st.st_mode))
    syslog(LOG_ERR,"%s file check failed\n",path);

  //获取文件描述符
  fd = open(path,O_RDONLY);
  if(fd < 0){
    syslog(LOG_ERR,"%s file check failed",path);
    return -1;
  }
  return fd;
}

//推动状态机
void fsm_driver(fsm_udp_t *fsm){
  switch(fsm->state){
    case STATE_R_PATH:
      //收path
      fsm->read_count = recvfrom(fsm->sockfd,&fsm->packet.u_path,sizeof(fsm->packet.u_path),0,(void *)&fsm->raddr,&fsm->raddrlen);
      if(fsm->read_count < 0){ //出错处理
        syslog(LOG_ERR,"recvfrom() faied.\n");
        fsm->state = STATE_Ex;
        break;
      }

      if(fsm->packet.u_path.mtype != MSG_PATH){ //判断收到的是否是 PATH 包
        syslog(LOG_ERR,"receive packet is not path.\n");
        fsm->state = STATE_R_PATH;
        break;
      }
 
            
      //文件检查
      fsm->fd = file_check(fsm->packet.u_path.path);
      if(fsm->fd < 0){
        syslog(LOG_ERR,"path is not a regular file.\n");
        fsm->state = STATE_R_PATH;
        break;
      }

      //包通过检查，recv_ackno 初始化为对端传递过来的 ackno
      fsm->recv_ackno = fsm->packet.u_path.ackno;

      //发ack
      fsm->packet.u_ack.mtype = MSG_ACK;
      fsm->packet.u_ack.ackno = ++fsm->local_ackno;

      //收到 path 后发送 ack 包
      if(sendto(fsm->sockfd,&fsm->packet.u_ack,sizeof(fsm->packet.u_ack),0,(void *)&fsm->raddr,sizeof(fsm->raddr)) < 0){
        syslog(LOG_ERR,"sendto() failed.\n");
        fsm->state = STATE_Ex;
        break;
      }
 
      //发送 ack 后进入 send data 状态
      fsm->state = STATE_S_DATA;
      break;
    
    case STATE_S_DATA:
      memset(&fsm->packet,'\0',sizeof(fsm->packet));
      fsm->read_count = read(fsm->fd,fsm->packet.u_data.data,sizeof(fsm->packet.u_data.data));
      fsm->count = 0; //每次读之前初始化超时计数
      if(fsm->read_count < 0){
        syslog(LOG_ERR,"read failed\n");
        fsm->state = STATE_Ex;
        break;
      }else if(fsm->read_count == 0){
        //发 eot
        fsm->packet.u_eot.mtype = MSG_EOT;
        fsm->packet.u_eot.ackno = fsm->local_ackno + 1;
        while(fsm->count < MAX_RETRY_TIMES){
          FD_ZERO(&fsm->rfds);
          FD_SET(fsm->sockfd,&fsm->rfds);
          fsm->tv.tv_sec = 5;
          fsm->tv.tv_usec = 0;
          syslog(LOG_ERR,"start send eot.\n");
          if(sendto(fsm->sockfd,&fsm->packet.u_eot,sizeof(fsm->packet.u_eot),0,(void *)&fsm->raddr,sizeof(fsm->raddr)) < 0){
            syslog(LOG_ERR,"sendto(): send eot failed\n");
            fsm->state = STATE_Ex;
            break;
          }
          syslog(LOG_ERR,"end end eot.\n");

          //超时重试
          fsm->ret = select(fsm->sockfd+1,&fsm->rfds,NULL,NULL,&fsm->tv);
          if(fsm->ret < 0){
            syslog(LOG_ERR,"select() failed.\n");
            fsm->state = STATE_Ex;
            break;
          }else if(fsm->ret == 0){
       
            syslog(LOG_INFO,"timeout,retry...");
            syslog(LOG_INFO,"retry local_ackno = %ld\n",fsm->local_ackno);
            fsm->count++;
            continue;
          }

          syslog(LOG_ERR,"start receive ack.\n");
          if(recvfrom(fsm->sockfd,&fsm->packet.u_ack,sizeof(fsm->packet.u_ack),0,(void *)&fsm->raddr,&fsm->raddrlen) < 0){
            syslog(LOG_ERR,"recvfrom(): receive ack failed\n");
            fsm->state = STATE_Ex;
            break;
          }
        
          if(fsm->packet.u_ack.ackno != (fsm->recv_ackno+1)){
            syslog(LOG_INFO,"expect receive ack but receive packet is not ack.\n");
            fsm->count++;
            continue;
          }
          fsm->local_ackno++;
          fsm->recv_ackno++;
          fsm->state = STATE_R_PATH;
          syslog(LOG_ERR,"end receive ack.\n");
          break;
        }
        if(fsm->count == MAX_RETRY_TIMES){
          fsm->state = STATE_Ex;
          syslog(LOG_ERR,"exceed max retry times..\n");
        }
        break;
      }else{
        //发data
        fsm->packet.mtype = MSG_DATA;
        fsm->packet.u_data.ackno = fsm->local_ackno + 1;
        fsm->packet.u_data.len = fsm->read_count; //保存数据包的长度
              
        while(fsm->count < MAX_RETRY_TIMES){
          FD_ZERO(&fsm->rfds);
          FD_SET(fsm->sockfd,&fsm->rfds);
          fsm->tv.tv_sec = 5;
          fsm->tv.tv_usec = 0;
          syslog(LOG_ERR,"start send data.");
          if(sendto(fsm->sockfd,&fsm->packet.u_data,sizeof(fsm->packet.u_data),0,(void *)&fsm->raddr,sizeof(fsm->raddr)) < 0){
            syslog(LOG_ERR,"sendto(): send data failed\n");
            fsm->state = STATE_Ex;
            break;
          }
          syslog(LOG_ERR,"end send data.\n");
        
          fsm->ret = select(fsm->sockfd+1,&fsm->rfds,NULL,NULL,&fsm->tv);
          if(fsm->ret < 0){
            syslog(LOG_ERR,"select() failed.\n");
            fsm->state = STATE_Ex;
            break;
          }else if(fsm->ret == 0){
            //超时
            syslog(LOG_INFO,"timeout,retry...");
            syslog(LOG_INFO,"retry local_ackno = %ld\n",fsm->local_ackno);
            fsm->count++;
            continue;
          }

          //收 ack
          syslog(LOG_ERR,"start receive ack.\n");
          if(recvfrom(fsm->sockfd,&fsm->packet.u_ack,sizeof(fsm->packet.u_ack),0,(void *)&fsm->raddr,&fsm->raddrlen) < 0){
            syslog(LOG_ERR,"recvfrom(): receive ack failed\n");
            fsm->state = STATE_Ex;
            break;
          }
          //如果收到的包不是期望的包，则重试继续发送data
          if(fsm->packet.u_ack.ackno != (fsm->recv_ackno+1)){
            syslog(LOG_INFO,"expect receive ack but receive packet is not ack.\n");
            fsm->count++;
            continue;
          }
          fsm->local_ackno++;
          fsm->recv_ackno++;
          fsm->state = STATE_S_DATA;
          syslog(LOG_ERR,"end receive ack.\n");
          break;
        }
        if(fsm->count == MAX_RETRY_TIMES){
          syslog(LOG_ERR,"exceed max retry times.\n");
          fsm->state = STATE_R_PATH;
          break;
        }
      }
      break;
    case STATE_Ex:
      ;
      break;
   
    case STATE_T:
      ;
      break;
    default:
      syslog(LOG_ERR,"fsm state abnormal\n");
      fsm->state = STATE_Ex;
  }

}

int main(){
  fsm_udp_t fsm;

  //服务端进程后台运行
  daemonize();

  //地址结构初始化
  fsm.laddr.sin_family = AF_INET;
  fsm.laddr.sin_port = htons(2888);
  if(inet_pton(AF_INET,"172.19.229.90",&fsm.laddr.sin_addr) < 0){
    syslog(LOG_ERR,"inet_pton() failed\n");
    exit(1);
  }

  //创建 socket
  fsm.sockfd = socket(AF_INET,SOCK_DGRAM,0);
  if(fsm.sockfd < 0){
    syslog(LOG_ERR,"socket() failed\n");
    exit(1);
  }

  //socket绑定端口
  if(bind(fsm.sockfd,(void *)&fsm.laddr,sizeof(fsm.laddr)) < 0){
    syslog(LOG_ERR,"bind() failed\n");
  }

  //初始化raddrlen
  fsm.raddrlen = sizeof(fsm.raddr);
  fsm.local_ackno = 0;


  while(fsm.state != STATE_Ex && fsm.state != STATE_T)
    fsm_driver(&fsm);

  if(fsm.state == STATE_Ex){
    syslog(LOG_ERR,"process exit abnomal.\n");
  }else if(fsm.state == STATE_T){
    syslog(LOG_ERR,"process exit normal.\n");
  }

  exit(0);
}

