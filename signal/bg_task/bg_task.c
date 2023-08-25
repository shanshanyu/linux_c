#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


void sighup_handler(int signo){
  fprintf(stdout,"sighup catch: %d",signo);
}


int main(){
  int fd;
  sigset_t set;
  struct sigaction sa;

  if(sigemptyset(&set)  == -1){
    perror("sigemtpyset()");
    exit(1);
  }

  sa.sa_handler = sighup_handler;
  sa.sa_mask = set;
  sa.sa_flags = 0;
  
  sigaction(SIGHUP,&sa,NULL);

  fd = open("bg.out",O_WRONLY|O_CREAT|O_TRUNC,0644);
  if(fd < 0){
    perror("open()");
    exit(1);
  }

  dup2(fd,1);
  dup2(fd,2);

  pause();  

  exit(0);
}
