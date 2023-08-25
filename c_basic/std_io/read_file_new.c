#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024

void read_file(const char *path){
  FILE *fp = NULL;
  char buf[BUFSIZE];

  //打开文件
  fp = fopen(path,"r");
  if(fp == NULL){
    perror("fopen()");
    exit(1);
  } 
  
  while(fgets(buf,BUFSIZE,fp) != NULL){
    if(fputs(buf,stdout) == EOF){
      fprintf(stderr,"fputs() failed\n");
      exit(1);
    }
  }
  
  //如果是需要返回值，可以把这里改一下，return 返回值
  if(feof(fp)){
    fprintf(stderr,"read success\n");
  }else if(ferror(fp)){
    fprintf(stderr,"read failed\n");
  } 

  fclose(fp);
}


int main(int argc,const char *argv[]){
  if(argc != 2){
    fprintf(stderr,"Usage: %s filename\n",argv[0]);
    exit(1);
  }

  read_file(argv[1]);
  exit(0);
}
