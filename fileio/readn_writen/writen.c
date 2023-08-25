#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>



#define BUFSIZE 1024
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

//从 fd 中读 n 个字节到 buf 中
int writen(int fd,char *buf,int n){
  int n_write,n_left,pos;
  pos = 0;
  n_left = n;

  while(n_left > 0){
    n_write = write(fd,buf+pos,n_left); //第一次没读完，第二次读不应该从buf开始
    if(n_write < 0){
      if(errno == EINTR)
        continue;
      else{
        //如果一个字节没读到，出错返回-1
        if(n_left == n)
          return -1;
        else 
          break;
      }  
    }
    if(n_write == 0)  //考虑 write 返回值为 0 的这种情况，n_left == 0,如果确定不会将 n_left传0，不需要考虑这种情况！！!
      break;

   n_left -= n_write;
   pos += n_write;
  }
  return pos;
}

int main(){
  int n_write,n_read;
  char buf[BUFSIZE];
  int fd_r,fd_w;
  fd_r = open("/etc/services",O_RDONLY);
  fd_w = open("/tmp/out",O_WRONLY|O_CREAT|O_TRUNC,0644);
  n_read = readn(fd_r,buf,BUFSIZE);
  n_write = writen(fd_w,buf,n_read);
  printf("n_read = %d,n_write = %d\n",n_read,n_write);
  exit(0);
}
