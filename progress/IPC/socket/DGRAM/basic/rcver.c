#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "proto.h"

//服务端程序

int main(){
  int sockfd;
  struct sockaddr_in laddr,raddr;//地址
  char *addr = "0.0.0.0";
  int ret;
  socklen_t raddr_len; //对端地址长度
  struct msg_st msg;
  
  char plain_addr[INET_ADDRSTRLEN]; 

  //创建套接字
  sockfd = socket(AF_INET,SOCK_DGRAM,0); //protocol 可以写成 IPPROTO_UDP
  if(sockfd == -1){
    perror("socket()");
    exit(1);
  }
  
  //初始化 sockaddr_in 地址结构
  laddr.sin_family = AF_INET;
  laddr.sin_port = htons(2000); //端口转换成网络字节序
  ret = inet_pton(AF_INET,addr,&laddr.sin_addr);
  if(ret <= 0){
    if(ret == 0){
      fprintf(stderr,"Not a valid address\n");
    }else{
      perror("inet_pton()");
    }  
    exit(1);
  }

  //地址绑定
  if(bind(sockfd,(struct sockaddr *)&laddr,sizeof(laddr)) == -1){
    perror("bind()");
    exit(1);
  }

  //printf("%u",laddr.sin_addr.s_addr);

  raddr_len = sizeof(raddr);
  while(1){
    ret = recvfrom(sockfd,&msg,sizeof(msg),0,(void *)&raddr,&raddr_len);
    if(ret == 0)
      break;
    else if(ret == -1){
      perror("recvfrom()");
      exit(1);
    }else{
      printf("receive msg from : %s:%u\n",inet_ntop(AF_INET,(void *)&raddr.sin_addr,plain_addr,INET_ADDRSTRLEN),ntohs(raddr.sin_port));
      printf("name: %s\n",msg.name);
      printf("math: %u\n",ntohl(msg.math));
      printf("chinese: %u\n",ntohl(msg.chinese));
    }
  }
  close(sockfd);
  exit(0);
}
