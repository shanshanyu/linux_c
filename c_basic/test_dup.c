#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Q: 测试 dup，关闭文件时会不会关闭文件表项？
//A: 不会


int main(){
  int fd,dup_fd;

  fd = open("/tmp/out1",O_RDWR|O_CREAT|O_TRUNC,0644);
  if(fd < 0){
    perror("open()");
    exit(1);
  }

  dup_fd = dup(fd);
  
  write(fd,"fd write",8);

  write(dup_fd,"dup_fd write",12);

  close(fd);

  write(dup_fd,"dup_fd write 2",14);

  exit(0);
}
