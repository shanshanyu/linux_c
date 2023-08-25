#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(){
  if(mkfifo("/tmp/out1",0644) != 0){
    perror("mkfifo()");
    exit(1);
  }

  char buf[5];
  int fd;
  fd = open("/tmp/out1",O_RDWR);
  if(fd < 0){
    perror("open()");
    exit(1);
  }

  write(fd,"hello",5);
  read(fd,buf,5);

  puts(buf);
  exit(0);
}
