#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024

//read_file应该是有返回值的
void read_file(const char *path){
  FILE *fp = NULL;
  char buf[BUFSIZE];
  //打开文件
  fp = fopen(path,"r");
  if(fp == NULL){
    perror("fopen()");
    exit(1);
  } 
  
  
  //这段逻辑有问题：
  //对于错误的场景没做判断，fgets返回NULL可能有两种情况，一种文件结尾，一种是出错，如果是出错，这个程序判断不了
  fgets(buf,BUFSIZE,fp);
  while(feof(fp) == 0){
    fputs(buf,stdout);
    fgets(buf,BUFSIZE,fp);
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
