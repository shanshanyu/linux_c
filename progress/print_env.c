#include <stdio.h>
#include <stdlib.h>

//打印环境表

extern char **environ;  //environ 是指向指针的指针变量。指向一个指针数组。指针的运算：加减运算

int main(){

  while(*environ != NULL){
    printf("%s\n",*environ);
    environ++;
  }

  exit(0);
}
