#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFSIZE 1024


void read_file(const char *path){
  int fd;
  char buf[BUFSIZE];
  fd = open(path,O_RDONLY);
  int ret;

  while (1){
    ret = read(fd,buf,BUFSIZE);
    if(ret < 0){
      perror("read()");
      exit(1);
    }else if(ret == 0)
      break;
    else{
      if(write(STDOUT_FILENO,buf,ret) != ret){
        perror("write()");
        exit(1); 
      }
    }
  }

}


int main(int argc,const char *argv[]){
  if(argc != 2){
    fprintf(stderr,"Usage:%s filename\n",argv[0]);
    exit(1);
  }

  read_file(argv[1]);

  exit(0);
}
