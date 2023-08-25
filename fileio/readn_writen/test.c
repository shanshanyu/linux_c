#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>



#define BUFSIZE 1024
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

//测试 write 写0 个字符的情况

int main(){
  char buf[BUFSIZE];
  int fd_w;
  int fd_r;
  int n_write;
  fd_r = open("/etc/services",O_RDONLY);
  if(fd_r < 0){
    perror("open()");
    exit(1);
  }
  printf("n_read = %d\n",read(fd_r,buf,0));
  fd_w = open("/tmp/out",O_WRONLY|O_CREAT|O_TRUNC,0644);
  if(fd_w < 0){
    perror("open()");
    exit(1);
  }
  n_write = write(fd_w,buf,0);
  if(n_write < 0){
    perror("write()");
    exit(1);
  }

  printf("n_write = %d\n",n_write);
  exit(0);
}
