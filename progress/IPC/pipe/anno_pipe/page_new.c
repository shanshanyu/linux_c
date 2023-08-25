#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024

int main(int argc,const char *argv[]){
  //判断参数个数
  if(argc != 2){
    fprintf(stderr,"usage:%s filename\n",argv[0]);
    exit(0);
  }

  FILE *fpin,*fpout;
  char buf[BUFSIZE];
  char *s;
  fpout = popen("more","w");
  if(fpout == NULL){
    fprintf(stderr,"popen() failed\n");
    exit(1);
  }

  fpin = fopen(argv[1],"r");
  if(fpin == NULL){
    perror("fopen()");
    exit(1);
  }
  
  //读文件,fpin的数据放入到fpout中
  while((s = fgets(buf,BUFSIZE,fpin)) != NULL){
    if(fputs(buf,fpout) == EOF){
      fprintf(stderr,"fputs() err\n");
      exit(1);
    }
  }
  
  if(ferror(fpin)){
    fprintf(stderr,"ferror() err\n");
    exit(1);
  }  

  if(pclose(fpout) == -1){
    fprintf(stderr,"pclose() err\n");
    exit(1);
  }
  fclose(fpin);
  exit(0);
}
