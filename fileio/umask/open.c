#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){
  //使用open创建要给文件 /tmp/out，设置文件权限，看看是怎么作用的？
  int fd;
  fd = open("/tmp/out",O_RDWR|O_CREAT|O_TRUNC,0777);  //mode & (~umask)  0777 & 7775  0775,这个是文件最大的权限了 0775
  if(fd < 0){
    perror("open()");
    exit(1);
  }

  exit(0);
}
