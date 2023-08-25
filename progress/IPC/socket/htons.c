#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

//测试网络字节序和主机字节序的不同


int main(){
  unsigned short int a= 0x1234;  
  printf("主机字节序 a = %x \n",a);

  unsigned short int b;
  b = htons(a);
  
  printf("网络字节序 a = %x \n",b);
 
  printf("2000 的主机字节序转网络字节序: %d\n",htons(2000));
  exit(0);
}
