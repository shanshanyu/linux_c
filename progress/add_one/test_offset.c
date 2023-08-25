#include <stdio.h>
#include <stdlib.h>

//一个文件打开2次，它们的 offset 是否共用?也就是它们的文件表项是否共享

#define BUFSIZE 1024
int main(){
  char buf[BUFSIZE];
  FILE *fp1,*fp2;
  fp1 = fopen("/tmp/out","r+");
  if(fp1 == NULL){

  }

  fgets(buf,BUFSIZE,fp1);
  printf("fp1 current offset:%d\n",ftell(fp1)); 
  fp2 = fopen("/tmp/out","r+");
  printf("fp2 current offset:%d\n",ftell(fp2)); //0 说明2个 fd不共用同一个文件表项

  exit(0);
}
