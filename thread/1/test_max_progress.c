#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){

  long int i = 0;
  pid_t pid;
  while(1){
    pid = fork();
    if(pid < 0){
      perror("fork()");
      break;
    }else if(pid == 0){
      exit(0);
    }else{
      ;
    }
    i++;
  }
  printf("当前环境可以创建的进程数：%ld\n",i);
}
