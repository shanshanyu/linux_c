#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(){
  //保存之前的umask值
  mode_t u_save;
  int err;
  u_save = umask(0);
  err = creat("/tmp/out1",0777); //文件的权限应该是 0777
  if(err < 0){
    perror("creat()");
    exit(1);
  }

  umask(S_IRWXG|S_IRWXO); //屏蔽所属组和其他人权限
  err = creat("/tmp/out2",0777); //文件的权限应该是 0700
  if(err < 0){
    perror("creat()");
    exit(1);
  }
  //还原之前的文件模式屏蔽字
  umask(u_save);
  printf("over\n");
  exit(0);
}
