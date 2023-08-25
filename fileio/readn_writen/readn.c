#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>



#define BUFSIZE 1024
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

//从 fd 中读 n 个字节到 buf 中
int readn(int fd,char *buf,int n){
  int n_read,n_left,pos;
  pos = 0;
  n_left = n;

  while(n_left > 0){
    n_read = read(fd,buf+pos,n_left); //第一次没读完，第二次读不应该从buf开始
    if(n_read < 0){
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

   if(n_read == 0) break;
   n_left -= n_read;
   pos += n_read;
  }
  return pos;
}

/*int main(){
  int n_read;
  char buf[BUFSIZE];
  int fd;
  fd = open("/etc/services",O_RDONLY);
  n_read = readn(fd,buf,BUFSIZE);
  printf("n_read = %d\n",n_read);
  exit(0);
}*/

