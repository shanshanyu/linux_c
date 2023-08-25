#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(){
  struct sockaddr_in raddr;
  int sockfd;
  char buf[BUFSIZE];
  
  sockfd = socket(AF_INET,SOCK_STREAM,0);
  if(sockfd < 0){
    perror("socket()");
    exit(1);
  }

  raddr.sin_family = AF_INET;
  raddr.sin_port = htons(2000);
  inet_pton(AF_INET,"172.19.229.90",&raddr.sin_addr);
 

  if(connect(sockfd,(void *)&raddr,sizeof(raddr)) < 0){
    perror("connect()");
    exit(1);
  }

  send(sockfd,"hello",5,0);
  //recv(sockfd,buf,BUFSIZE,0);
  //puts(buf);

  close(sockfd);
  
  exit(0);
}
