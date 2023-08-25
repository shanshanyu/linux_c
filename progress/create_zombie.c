#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//创建10个僵尸进程


int main(){
  pid_t pid;
  int i;
  for(i = 0; i < 10; ){
    pid = fork();
    if(pid < 0){
      perror("fork()");
      exit(1);
    }
    else if(pid == 0)
      exit(0);
    else
      ;
  }

  pause();

  exit(0);
}

