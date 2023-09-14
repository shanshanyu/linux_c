#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


void err_msg(const char *s){
  fprintf(stderr,"err: %s\n",s);
  exit(1);
}

int main(){
  int listenfd,connfd;
  struct sockaddr_in servaddr;

  //创建一个 socket
  listenfd = socket(AF_INET,SOCK_STREAM,0);
  if(listenfd == -1)
    err_msg("create socket failed");
  
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(3333);
  if(inet_pton(AF_INET,"172.19.229.90",&servaddr.sin_addr) <= 0)
    err_msg("inet_pton failed");

  //bind 
  if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr))  == -1)
    err_msg("bind failed");

  //listen
  if(listen(listenfd,128) == -1)
    err_msg("listen failed");

  while(1){
    connfd = accept(listenfd,NULL,NULL);
    if(connfd == -1)
      err_msg("accept failed");
    
    write(connfd,"你是一个大傻逼",100);
    close(connfd);
  }
  

  exit(0);
}
