#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 *用宏测试文件类型
 *
 */


int main(int argc,const char *argv[]){
  if(argc != 2){
    fprintf(stderr,"Usage:%s filename\n",argv[0]);
    exit(1);
  }
  struct stat st;
  if(stat(argv[1],&st) == -1){
    perror("stat failed");
    exit(1);
  }

  if(S_ISREG(st.st_mode))
    printf("regular file\n");
  else if(S_ISDIR(st.st_mode))
    printf("dir\n");
  else if(S_ISCHR(st.st_mode))
    printf("character device\n");
  else if(S_ISBLK(st.st_mode))
    printf("block device\n");
  else if(S_ISFIFO(st.st_mode))
    printf("FIFO\n");
  else if(S_ISLNK(st.st_mode))
    printf("symbolic link\n");
  else if(S_ISSOCK(st.st_mode))
    printf("socket\n");
  else
    printf("unrecognize\n");

  printf("st_dev = %d\n",st.st_dev);
  printf("st_ino = %ld\n",st.st_ino);
  

  return 0;
}
