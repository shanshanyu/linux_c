#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main(){
  unsigned char buf[sizeof(struct in6_addr)]; //用这个格式来保存二进制地址格式（网络字节序）
  int ret;
  unsigned int addr;
  char str[INET_ADDRSTRLEN];
  addr = htonl(INADDR_ANY); //转换成网络字节序的二进制地址
  if(inet_ntop(AF_INET,&addr,str,INET_ADDRSTRLEN) == NULL){
    perror("inet_ntop()");
    exit(1);
  }
  puts(str);

  ret = inet_pton(AF_INET,"0.0.0.0",buf);
  if(ret <= 0){
    if(ret == 0){
      fprintf(stderr,"Address is not valid\n");
    }else
      perror("inet_pton()");
    exit(1);
  }

  
  exit(0);
}
