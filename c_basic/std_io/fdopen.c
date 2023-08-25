#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 *标准io使用的type需要小于等于open打开文件时的权限
  
**

*/


int main(){
  int fd;
  FILE *fp;

  fd = open("/tmp/out",O_WRONLY|O_CREAT|O_TRUNC,0644);
  if(fd < 0){
    perror("open()");
    exit(1);
  }

  fp = fdopen(fd,"r");
  if(fp == NULL){
    perror("fdopen()");
    exit(1);
  }

  exit(0);
}
