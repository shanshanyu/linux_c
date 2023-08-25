#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main(int argc,const char *argv[]){
  int socketfd;  
  struct sockaddr_in servaddr;


  if (argc != 2){
    fprintf(stderr,"Usage:%s <Ipaddress>");
    exit(1); 
  }

  //创建 socket
  if((socketfd = socket(AF_INET,SOCK_DGRAM,0)) < 0){
    fprintf(stderr,"socket error");
    exit(1);
  }

  memset(&servaddr,'\0',sizeof(servaddr));

  servaddr.sin

}
