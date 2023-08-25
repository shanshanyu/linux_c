#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define TTY1 "/dev/tty11"

//select 会阻塞当前进程，直到有感兴趣的事件发生


typedef struct utp_fsm_st{
  fd_set rfds;
  struct timeval tv;
  int fd;
}utp_fsm_t;

int main(){
    
          utp_fsm_t fsm;
  fsm.fd = open(TTY1,O_RDONLY);
  if(fsm.fd < 0){
    perror("open()");
    exit(1);
  }
          FD_ZERO(&fsm.rfds);
          FD_SET(fsm.fd,&fsm.rfds);
          fsm.tv.tv_sec = 5;
          fsm.tv.tv_usec = 0;

          select(0,NULL,NULL,NULL,&fsm.tv);
          
          fprintf(stderr,"hello.\n");







  exit(0);
}
