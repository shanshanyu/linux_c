#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

//测试点分十进制地址和二进制地址的区别


int main(){
  
  char *str = "192.168.1.1";

  struct in_addr sin_addr;

  if(inet_pton(AF_INET,str,&sin_addr) != 1){
    perror("inet_pton()");
    exit(1);
  }

  printf("inet_pton: 0x%x \n",sin_addr.s_addr);

  exit(0);
}
