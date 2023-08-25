#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//该程序调用system函数，并且设置用户id位为root

int main(int argc,char *argv[]){
  if(argc != 2){
    fprintf(stderr,"Usage:%s args\n",argv[0]);
    exit(0);
  }
  int err;

  err = system(argv[1]);
  if(err == -1){
    fprintf(stderr,"system() error\n");
    exit(1);
  }
  
  exit(0);
}
