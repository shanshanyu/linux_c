#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROCNUM 20

#define FILENAME "/tmp/out"
#define BUFSIZE 1024

void add_one(void){
  FILE *fp;
  int fd;
  char buf[BUFSIZE];
  //以读写方式打开
  fp = fopen(FILENAME,"r+");
  if(fp == NULL){
    perror("fopen()");
    exit(1);
  }

  //文件加锁
  fd = fileno(fp);
  if(lockf(fd,F_LOCK,0) < 0){
    perror("lockf()");
    exit(1);
  }

  fgets(buf,BUFSIZE,fp);

  if(fseek(fp,0,SEEK_SET) == -1){
    perror("fseek()");
    exit(1);
  }
  fprintf(fp,"%d",atoi(buf)+1);
  fflush(fp);

  //解锁
  if(lockf(fd,F_ULOCK,0) < 0){
    perror("lockf()");
    exit(1);
  }
  fclose(fp);
}


int main(){
  pid_t pid; //不需要pid数组，因为回收子进程不需要提供pid号，直接用wait回收
  int i;
  //创建子进程，让子进程取执行add_one
  for(i = 0; i < PROCNUM; i++){
    pid = fork();
    if(pid < 0){
      perror("fork()");
      exit(1);
    }
    if(pid == 0){
      add_one();
      exit(0);
    }
  } 
  
  //回收进程资源
  for(i = 0; i < PROCNUM; i++){
    wait(NULL);
  }
 
  exit(0);
}
