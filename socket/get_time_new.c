#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_LINE 1024

int main(int argc,const char *argv[]){
  int socketfd;  
  struct sockaddr_in servaddr;

  char recvline[MAX_LINE];
  int n;


  if (argc != 2){
    fprintf(stderr,"Usage:%s <Ipaddress>\n",argv[0]);
    exit(1); 
  }

  //创建 socket
  if((socketfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
    fprintf(stderr,"socket error");
    exit(1);
  }

  memset(&servaddr,'\0',sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(13);  //13 是什么端口？
  if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr) <= 0){
    fprintf(stderr,"%s 地址格式无效\n",argv[1]);
    exit(1);
  }

  if(connect(socketfd,(void *)&servaddr,sizeof(servaddr)) == -1){
    fprintf(stderr,"connect error\n");
    exit(1);
  }

  while(1){
    n = read(socketfd,recvline,MAX_LINE-1);
    if(n < 0){
      fprintf(stderr,"read error\n");
      exit(1);
    }

    if(n == 0) break;
    recvline[n] = 0;
    if(fputs(recvline,stdout) == EOF){
       fprintf(stderr,"fputs error\n");
       exit(1);
     }
  }
  
  if(n < 0){
    fprintf(stderr,"read error\n");
    exit(1);
  }
  exit(0);
}
