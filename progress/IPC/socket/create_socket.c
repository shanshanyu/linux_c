#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

//创建一个套接字

int main(){
  int fd;
  fd = socket(AF_INET,SOCK_STREAM,0);
  if(fd < 0){
    perror("socket");
    exit(1);
  }
  printf("socket fd: %d\n",fd);
  sleep(60);
  exit(0);
}
