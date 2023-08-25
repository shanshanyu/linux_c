#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "my_fsm.h"
#include "relay.h"

#define tty1 "/dev/tty9"
#define tty2 "/dev/tty10"
#define tty3 "/dev/tty11"
#define tty4 "/dev/tty12"


 
int main(){
  int fd1,fd2,fd3,fd4;
  int err;
  fd1 = open(tty1,O_RDWR);
  if(fd1 < 0){
    perror("open()");
    exit(1);
  }
  write(fd1,"tty1\n",5);

  fd2 = open(tty2,O_RDWR);
  if(fd2 < 0){
    perror("open()");
    exit(1);
  }
  write(fd2,"tty2\n",5);

  err =  add_job(fd1,fd2);
  if(err < 0){
    fprintf(stderr,"add_job failed:%s\n",strerror(-err));
    exit(1);
  }

  fd3 = open(tty3,O_RDWR);
  if(fd1 < 0){
    perror("open()");
    exit(1);
  }
  write(fd3,"tty3\n",5);

 fd4 = open(tty4,O_RDWR);
  if(fd1 < 0){
    perror("open()");
    exit(1);
  }
  write(fd4,"tty4\n",5);

  err =  add_job(fd3,fd4);
  if(err < 0){
    fprintf(stderr,"add_job failed:%s\n",strerror(-err));
    exit(1);
  }
  while(1)
    pause();
  close(fd1);
  close(fd2);
  close(fd3);
  close(fd4);
  exit(0);
}
