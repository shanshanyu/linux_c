#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define BUFSIZE  1024

int main(int argc,const char *argv[]){
  //要求传入 filename
  if(argc != 2){
    fprintf(stderr,"%s filename\n",argv[0]);
    exit(1);
  }

  pid_t pid;
  FILE *fp;
  char buf[BUFSIZE];
  int n;
  int fd[2];
  //创建管道
  if(pipe(fd) == -1){
    perror("pipe()");
    exit(1);
  }

  //打开文件
  fp = fopen(argv[1],"r");
  if(fp == NULL){
    perror("fopen()");
    exit(1);
  }

  //创建子进程
  pid = fork();
  if(pid < 0){
    perror("fork()");
    exit(1);
  }else if(pid == 0){
    //关闭写端
    close(fd[1]);
    if(fd[0] != STDIN_FILENO){
      //dup2成功后关闭fd[0],管道的读端指向标准输入
      if(dup2(fd[0],STDIN_FILENO) == -1){
        perror("dup2()");
        exit(1);
      }else
        close(fd[0]);
    }

   execl("/usr/bin/more","more",NULL);
   fprintf(stderr,"execl() err");
   exit(1);

  }else{
    //父进程的逻辑是把文件中所有的数据写入管道
    //关闭读端
    close(fd[0]);
    //标准 IO 读文件
    while(fgets(buf,BUFSIZE,fp) != NULL){
      n = strlen(buf);
      if(write(fd[1],buf,n) != n){
        perror("write()");
        exit(1);
      }
    }
    //如果
    if(ferror(fp)){
      fprintf(stderr,"ferror() failed\n");
      exit(1);
    }
    //所有数据读完关闭fd[1]
    close(fd[1]);
    //wait(NULL);
    waitpid(pid,NULL);
  }


  exit(0);
}
