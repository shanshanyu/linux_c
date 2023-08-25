#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFSIZE 1024

int main(int argc,const char *argv[]){
  if(argc != 2){
    fprintf(stderr,"Usage: %s filename\n",argv[0]);
    exit(1);
  }
  FILE *fp = fopen(argv[1],"r");
  if(fp == NULL){
    perror("open()");
    exit(1);
  }
  FILE *tmp_fp;
  pid_t pid;
  int fd[2];
  
  char buf[BUFSIZE];
  if(pipe(fd) < 0){
    perror("pipe()");
    exit(1);
  }
  pid = fork();
  if(pid < 0){
    perror("fork()");
    exit(1);
  }else if(pid == 0){
    //child
    close(fd[1]);
    //做一个判断
    if(fd[1] != STDIN_FILENO){
      if(dup2(fd[0],STDIN_FILENO) < 0){
        perror("dup2()");
        exit(1);
      }
    }
    close(fd[0]);
    execl("/bin/more","more",NULL);
  }else{
    //parent
    close(fd[0]);
    tmp_fp = fdopen(fd[1],"w");
    if(tmp_fp == NULL){
      perror("fdopen()");
      exit(1);
    }
    while(fgets(buf,BUFSIZE,fp) != NULL){
      fputs(buf,tmp_fp);
      //fflush(NULL);
    }
    
    if(ferror(fp)){
      fprintf(stderr,"fgets failed\n");
      exit(1);
    }
    close(fd[1]);
    wait(NULL);
  }

  return 0;
}
