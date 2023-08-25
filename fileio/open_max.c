#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  //获取当前用户的文件描述符的最大值
  printf("OPEN_MAX-1 = %ld\n",sysconf(_SC_OPEN_MAX)-1);
 

  exit(0);
}
