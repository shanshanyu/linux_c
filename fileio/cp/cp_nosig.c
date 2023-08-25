#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//cp file,no signal  慢速系统调用会被信号中断，磁盘不是慢速设备，不会被信号大端

#define BUFSIZE 1024

int main(int argc,const char *argv[]){
  if(argc != 3){
    fprintf(stderr,"Usage:%s srcfile dstfile\n",argv[0]);
    exit(1);
  }
  int fd_src,fd_dst;
  int n_read;
  char buf[BUFSIZE];
  fd_src = open(argv[1],O_RDONLY);
  if(fd_src < 0){
    perror("srcfile open()");
    exit(1);
  }
  fd_dst = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644);
  if(fd_dst < 0){
    perror("dstfile open()");
    exit(1);
  }

  while(1){
    n_read = read(fd_src,buf,BUFSIZE);
    if(n_read < 0){
      perror("read()");
      exit(1);
    }
    if(n_read == 0) break; //normal exit
    if(write(fd_dst,buf,n_read) != n_read){
      perror("write()");
      exit(1);
    }

  }
  
  exit(0);
}
