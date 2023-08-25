#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>



int main(){
  //linux系统中文件名的最长长度是 254 个字节
  printf("NAME_MAX-1 = %ld\n",pathconf("./",_PC_NAME_MAX)-1);

  //posix 要求的最小值是 14 个字节
  printf("%d\n",_POSIX_NAME_MAX);

  printf("_POSIX_NO_TRUNC = %d\n",_POSIX_NO_TRUNC);
  printf("_POSIX_NO_TRUNC = %ld\n",pathconf("./",_PC_NO_TRUNC));

  exit(0);
}
