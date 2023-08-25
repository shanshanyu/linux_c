#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*
 *创建一个文件，向文件中写入 123456789
 *读出 7
 *把 8 改成 6
 */


int main(){
  int fd;
  char *str = "123456789";

  char buf[1024] = {0};


  fd = open("/tmp/out1",O_RDWR|O_CREAT|O_TRUNC,0644);
  if(fd < 0){
    perror("open()");
    exit(1);
  }


  if(write(fd,str,strlen(str)) != strlen(str)){
    perror("write()");
    exit(1);  
  }

  lseek(fd,6,SEEK_SET);

  read(fd,buf,1);

  write(STDOUT_FILENO,buf,strlen(buf));


  lseek(fd,7,SEEK_SET);
  write(fd,"6",1);

  exit(0);
}
