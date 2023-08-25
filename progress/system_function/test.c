#include <stdio.h>
#include <stdlib.h>

int main(){
  //空指针调用system函数，测试system是否可用
  int err;
  err = system(NULL);
  if(err)
    printf("system valid\n");
  else
    printf("system invalid\n");

  exit(0);
}
