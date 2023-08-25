#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "proto.h"

//客户端程序   向广播地址上发送UDP报文数据

int main(){
  int sockfd;
  struct sockaddr_in raddr;
  struct msg_st msg;
  int ret;
  int opt = 1; //套接字选项中打开广播选项

  strcpy(msg.name,"tom");
  msg.math = htonl(80);
  msg.chinese = htonl(90);

  //地址初始化
  raddr.sin_family = AF_INET;
  raddr.sin_port = htons(2000);
  ret = inet_pton(AF_INET,"255.255.255.255",&raddr.sin_addr);
  if(ret <= 0){
    if(ret == 0)
      fprintf(stderr,"Not a valid address\n");
    else{
      perror("inet_pton()");
      exit(1);
    }
    exit(1);
  }

  //创建套接字
  sockfd = socket(AF_INET,SOCK_DGRAM,0);
  if(sockfd == -1){
    perror("socket()");
    exit(1);
  }

  //打开广播选项
  if(setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&opt,sizeof(opt)) == -1){
    perror("setsockopt()");
    exit(1);
  }


  if(sendto(sockfd,&msg,sizeof(msg),0,(struct sockaddr *)&raddr,sizeof(raddr))  == -1){
    perror("sendto()");
    exit(1);
  }
  
  printf("send success\n");

  exit(0);
}
