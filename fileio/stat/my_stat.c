#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc,const char *argv[]){
  if(argc != 2){
    fprintf(stderr,"usage:%s filename\n",argv[0]);
    exit(0);
  }
  struct stat  st;
  if(stat(argv[1],&st) == -1){
    perror("stat");
    exit(1);
  }

  switch(st.st_mode & S_IFMT){
    case S_IFSOCK: printf("is socket\n");break;
    case S_IFLNK: printf("is symbolic link\n"); break;
    case S_IFREG: printf("is regular file\n"); break;
    case S_IFBLK: printf("is block device\n"); break;
    case S_IFDIR: printf("is directory\n"); break;
    case S_IFCHR: printf("is character device\n"); break;
    case S_IFIFO: printf("is FIFO\n"); break;
    default: printf("unknow file\n");
  }

  exit(0);
}
