#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//测试 dup2,如果 fd2 等于 fd,fd2不会关闭？
//测试正常，复制 1，标准输出能正常输出

int main(){
  
  if(dup2(STDOUT_FILENO,1) < 0){
    perror("dup2()");
    exit(-1);
  }
  printf("dup2 ok\n");

  exit(0);
}
