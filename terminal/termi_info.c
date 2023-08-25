#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){
  int fd;
  fd = open("/dev/tty",O_RDONLY);
  if(fd < 0){
    perror("open()");
    exit(1);
  }
 
  if(isatty(fd) == 1){
    printf("/dev/tty is termianl\n");
  }else{
    printf("/dev/tty is not terminal\n");
  }

  printf("ttyname terminal name: %s\n",ttyname(fd));

  printf("ctermid terminal name: %s\n",ctermid(NULL));

  printf("_PC_VDISABLE = %ld\n",pathconf("/dev/tty",_PC_VDISABLE));

  exit(0);
}
