#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

//测试 select函数的第一个函数，看看 FD_SETSIZE 值

int main(){
  printf("FD_SETSIZE = %d\n",FD_SETSIZE); //1024

  exit(0);
}
