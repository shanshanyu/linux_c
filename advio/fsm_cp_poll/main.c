#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "my_fsm.h"

#define tty1 "/dev/tty11"
#define tty2 "/dev/tty12"


 
int main(){
  int fd1,fd2;
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
  relay(fd1,fd2);

  close(fd1);
  close(fd2);
  exit(0);
}
