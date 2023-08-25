#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


#define BUFSIZE 1024

int main(){
  int sockfd,client_sfd;
  struct sockaddr_in laddr;
  char buf[BUFSIZE];

  //创建socket
  sockfd = socket(AF_INET,SOCK_STREAM,0);
  if(sockfd < 0){
    perror("socket()");
    exit(1);
  }

  laddr.sin_family = AF_INET;
  laddr.sin_port = htons(2000);//tcp 绑定 2000 端口
  inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);

  //socket与地址绑定
  if(bind(sockfd,(void *)&laddr,sizeof(laddr)) < 0){
    perror("bind()");
    exit(1);
  }

  //宣告socket愿意接受连接
  if(listen(sockfd,128) < 0){
    perror("listen()");
    exit(1);
  }

  client_sfd = accept(sockfd,NULL,NULL);
  if(client_sfd < 0){
    perror("accept()");
    exit(1);
  }

  printf("client_sfd = %d\n",client_sfd);

  if(recv(client_sfd,buf,BUFSIZE,0) < 0){
    perror("recv()");
    exit(1);
  }
  puts(buf);

  //send(client_sfd,"hello",5,0);  

  close(sockfd);
  exit(0);
}
