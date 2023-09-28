#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFSIZE 1024

int main(){
  pid_t pid;
  int fd[2];
  char buf[BUFSIZE];
  int n;
  //由于pipe出错会设置 errno，所以用perror报错
  if(pipe(fd) == -1){
    perror("pipe()");
    exit(1);
  }

  pid = fork();
  if(pid < 0){
    perror("fork()");
    exit(1);
  }
  else if(pid == 0){ //子进程
    //子进程关闭写端
   if(close(fd[1]) == -1){
     perror("close()");
     exit(1);
   }
   //read write 逻辑
   n = read(fd[0],buf,BUFSIZE);
   write(STDOUT_FILENO,buf,n);
   
  }
  else{  //父进程
    //父进程关闭读端
    if(close(fd[0]) == -1){
      perror("close()");
      exit(1);
    }
    
    write(fd[1],"hello world\n",11);
    wait(NULL);
  }
  exit(0);
}
