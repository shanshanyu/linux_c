#include <stdio.h>
#include <stdlib.h>

//观察执行system 后的进程关系
//pause->cmd，进程调用system后，原理是调用了 fork/exec,exec 执行 /bin/sh -c "xxx"

int main(){

  int err;
  err = system("test");
  printf("err = %d\n",err);
  exit(127);

}
