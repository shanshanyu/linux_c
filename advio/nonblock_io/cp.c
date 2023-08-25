#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 500000

void set_fl(int fd,int flags){
//flags 表示需要打开的标志

  int val;
  val = fcntl(fd,F_GETFL);
  if(val == -1){
    fprintf(stderr,"fcntl failed\n");
    exit(1);
  }
  
  if(fcntl(fd,F_SETFL,val|flags) == -1){
    fprintf(stderr,"fcntl failed\n");
    exit(1);
  }
  printf("set flag success\n");
}

int main(){
  char buf[BUF_SIZE];
  int rd_ret,wr_ret;
  int pos;
  set_fl(STDOUT_FILENO,O_NONBLOCK);
while(1){
  rd_ret = read(STDIN_FILENO,buf,BUF_SIZE);
  if(rd_ret == -1){
    if(errno == EAGAIN)
      continue;
    else{
      perror("read()");
      exit(1);
    }
  }
  pos = 0;
  do{
    wr_ret = write(STDOUT_FILENO,buf,rd_ret);
    if(wr_ret == -1){
      if(errno == EAGAIN)
       continue;
      else{
        perror("write()");
        exit(1);
      }
    }
    pos += wr_ret;
  }while(pos < rd_ret);
}
  exit(0);
}
