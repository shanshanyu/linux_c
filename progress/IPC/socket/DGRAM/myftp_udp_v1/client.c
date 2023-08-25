#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/select.h>
#include "proto.h"

#define MAX_RETRY_TIMES 5 //最多重试 5 次

#define TIME_OUT 3  //3秒超时

//客户端状态
enum CLIENT_STATE{
  STATE_S_PATH,
  STATE_R_DATA,
  STATE_Ex,
  STATE_T
};

//客户端状态机结构
typedef struct fsm_udp_st{
  int state; 
  int sockfd;
  struct sockaddr_in laddr; 
  struct sockaddr_in raddr; 
  socklen_t raddrlen;
  union udp_s2c_st packet;
  long recv_ackno; 
  long local_ackno; 
  int dstfd;
  fd_set rfds;
  int count;
  struct timeval tv;
  int ret; //select 返回值
}fsm_udp_t;


void fsm_driver(fsm_udp_t *fsm){
  switch(fsm->state){
    case STATE_S_PATH:
      //包的初始化
      fprintf(stderr,"start send path.\n");
      fsm->packet.u_path.mtype = MSG_PATH;
      fsm->packet.u_path.ackno = fsm->local_ackno + 1;
      if(sendto(fsm->sockfd,&fsm->packet.u_path,sizeof(fsm->packet.u_path),0,(void *)&fsm->raddr,sizeof(fsm->raddr)) < 0){
        perror("sendto()");
        fsm->state = STATE_Ex;
        break;
      }
      fprintf(stderr,"end send path.\n");  


      fprintf(stderr,"start receive ack.\n");
      if(recvfrom(fsm->sockfd,&fsm->packet.u_ack,sizeof(fsm->packet.u_ack),0,(void *)&fsm->raddr,&fsm->raddrlen) < 0){
        perror("recvfrom()");
        fsm->state = STATE_Ex;
        break;
      }
      if(fsm->packet.u_ack.mtype != MSG_ACK){//收包的包不是期望的包
        fprintf(stderr,"receive packet is not ack..exit\n");
        fsm->state = STATE_T;
        break;
      }
      fprintf(stderr,"end receive ack.\n");     

      //包已经成功发出去并且接收到了确认包
      fsm->local_ackno++;
      fsm->recv_ackno = fsm->packet.u_ack.ackno; //remote_ackno 的第一次赋值
      fsm->state = STATE_R_DATA;
      break;

    case STATE_R_DATA:
      memset(&fsm->packet,'\0',sizeof(fsm->packet));
      //问题再这里，这里可鞥收到 eot 或 data，eot 只能根据 mtype 判断，不能根据 ackno 判断了，因为 ackno 在 len 里面了
      fprintf(stderr,"start receive data.\n");
      if(recvfrom(fsm->sockfd,&fsm->packet.u_data,sizeof(fsm->packet.u_data),0,(void *)&fsm->raddr,&fsm->raddrlen) < 0){
        perror("recvfrom()");
        fsm->state = STATE_Ex;
        break;
      }
    
      //收到的包是 eot
      if(fsm->packet.mtype == MSG_EOT){
        fprintf(stderr,"receive eot,start send ack.\n");
        fsm->packet.u_ack.mtype = MSG_ACK;
        fsm->packet.u_ack.ackno = fsm->local_ackno + 1;
        if(sendto(fsm->sockfd,&fsm->packet.u_ack,sizeof(fsm->packet.u_ack),0,(void *)&fsm->raddr,sizeof(fsm->raddr)) < 0){
          perror("sendto()");
          fsm->state = STATE_Ex;
          break;
        }
        fprintf(stderr,"send ack sucess.\n");
        fsm->local_ackno++;
        fsm->recv_ackno++;
        fsm->state = STATE_T;
        break;
      }
      
      if(fsm->packet.u_data.ackno != (fsm->recv_ackno + 1)){
        //说明 ack 丢失了
        if(fsm->packet.u_data.ackno == fsm->recv_ackno){
          fsm->packet.u_ack.mtype = MSG_ACK;
          fsm->packet.u_ack.ackno = fsm->local_ackno;
          if(sendto(fsm->sockfd,&fsm->packet.u_ack,sizeof(fsm->packet.u_ack),0,(void *)&fsm->raddr,sizeof(fsm->raddr)) < 0){
            perror("sendto()");
            fsm->state = STATE_Ex;
            break;
          } 
          fprintf(stderr,"send ack again.\n");
          fsm->state = STATE_R_DATA;
          break;
        } 
        fsm->state = STATE_Ex; //如果不是想要的包，程序退出
        fprintf(stderr,"packet is not wanted.exit!\n");
        break;
      }

     
      fprintf(stderr,"end receive data.\n");

      if(write(fsm->dstfd,fsm->packet.u_data.data,fsm->packet.u_data.len) != fsm->packet.u_data.len){
        perror("write()");
        fsm->state = STATE_Ex;
        break;
      }
      
      fprintf(stderr,"start send ack.\n");
      fsm->packet.u_ack.mtype = MSG_ACK;
      fsm->packet.u_ack.ackno = fsm->local_ackno + 1;
      if(sendto(fsm->sockfd,&fsm->packet.u_ack,sizeof(fsm->packet.u_ack),0,(void *)&fsm->raddr,sizeof(fsm->raddr)) < 0){
        perror("sendto()");
        fsm->state = STATE_Ex;
        break;
      }
      fprintf(stderr,"send ack sucess.\n");
      fsm->local_ackno++;
      fsm->recv_ackno++; 
      fsm->state = STATE_R_DATA;
      break;
    case STATE_Ex:
      ;
      break;
    case STATE_T:
      ;
      break;
    default:
      fprintf(stderr,"unknow fsm state\n");
      fsm->state = STATE_Ex;
  }
}


int main(int argc,const char *argv[]){
  if(argc != 3){
    fprintf(stderr,"Usage:%s srcfile(absolute path) dstfile\n",argv[0]);
    exit(1);
  } 

  fsm_udp_t fsm;

  fsm.dstfd = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0644);
  if(fsm.dstfd < 0){
    perror("open()");
    exit(1);
  }

  strcpy(fsm.packet.u_path.path,argv[1]);
  
  //laddr初始化
  fsm.raddr.sin_family = AF_INET;
  fsm.raddr.sin_port = htons(2888);
  if(inet_pton(AF_INET,"123.56.222.255",&fsm.raddr.sin_addr) < 0){
    syslog(LOG_ERR,"inet_pton() failed\n");
    exit(1);
  }

  //创建socket
  fsm.sockfd = socket(AF_INET,SOCK_DGRAM,0);
  if(fsm.sockfd < 0){
    syslog(LOG_ERR,"socket() failed\n");
    exit(1);
  }

  //fsm 成员初始化
  fsm.raddrlen = sizeof(fsm.raddr);
  fsm.state = STATE_S_PATH;
  fsm.local_ackno = 0;
  FD_SET(fsm.sockfd,&fsm.rfds); //设置文件描述符集，给 select 使用
  fsm.count = 0;
  fsm.tv.tv_sec = 5;
  fsm.tv.tv_usec = 0;  


  while(fsm.state != STATE_Ex && fsm.state != STATE_T)
    fsm_driver(&fsm);

  if(fsm.state == STATE_Ex)
    fprintf(stderr,"process exit abnormal.\n");
  if(fsm.state == STATE_T)
    fprintf(stderr,"process exit normal.\n");

  exit(0);
}
