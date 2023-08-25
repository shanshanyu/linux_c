#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
  int err;
  //当前的用户id情况
  printf("实际用户id uid = %d\n",getuid());
  printf("有效用户id uid = %d\n",geteuid());

  //设置用户id，看看 普通用户和root 用户调用 setuid 的区别
  //err = setuid(1001);
  err = setreuid(0,1200);
  if(err == -1){
    perror("setuid()");
    exit(1);
  }

  printf("实际用户id uid = %d\n",getuid());
  printf("有效用户id uid = %d\n",geteuid());

  exit(0);
}
