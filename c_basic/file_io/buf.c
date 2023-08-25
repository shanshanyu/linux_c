#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 1024

//验证 fclose 丢弃输入缓冲，刷新输出缓冲

int main(){
  int c;
  char buf[BUFSIZE];
  FILE *fp = fopen("/tmp/out","r");
  if(fp == NULL){
    perror("fopen()");
    exit(1);
  }
  
 // c = fgetc(fp);
  fgets(buf,BUFSIZE,fp);

  printf("hello");
  fclose(fp);
  sleep(20);

  //fputc(c,stderr);
  fputs(buf,stderr);

  fputc(c,stderr);


  exit(0);
}
