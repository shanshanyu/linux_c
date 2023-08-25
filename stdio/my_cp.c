#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFSIZE 1024
/*
 *复制文件
 *
 */



int main(int argc,const char *argv[]){
//判断传递的参数是否合理
  if(argc != 3){
    fprintf(stderr,"Usage:%s src_file dst_file\n",argv[0]);
    exit(1);
  } 
  FILE *src;
  FILE *dst;
  struct stat st;
  char dst_file[512];
  char buf[BUFSIZE];
  //打开源文件
  src = fopen(argv[1],"r");
  if(src == NULL){
     perror("fopen failed:");
     exit(1);
  }
  
  //判断目的文件是否目录
  if(stat(argv[2],&st) == -1){
    perror("stat failed:");
    exit(1);
  }
  //判断dst 的类型，看看是文件还是目录
  if(S_ISDIR(st.st_mode)){
    strcpy(dst_file,argv[2]);
    strcat(dst_file,"/");
    strcat(dst_file,argv[1]);
  }else if(S_ISREG(st.st_mode)){
    strcpy(dst_file,argv[2]);
  }else{
    fprintf(stderr,"%s not file or reg\n",argv[2]);
    exit(1);
  }
  dst = fopen(dst_file,"w");
  if(dst == NULL){
    perror("fopen failed:");
    exit(1);
  }

  //复制数据
  while(fgets(buf,BUFSIZE,src) != NULL){
    if(fputs(buf,dst) == EOF){
       fprintf(stderr,"fputs failed\n");
       exit(1);
    }
  }

  if(ferror(src)){
    fprintf(stderr,"fgets failed\n");
    exit(1);
  }
  printf("cp sucess\n");
  return 0;
}
