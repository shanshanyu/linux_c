#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  int i;
  pid_t pid;
  //创建 5 个子进程
  for(i = 0; i < 5; i++){
    pid = fork();
    if(pid < 0){
      perror("fork()");
      exit(1);
    }
    else if(pid == 0)
      break;
    else
      ;
  }

  if(i < 5) printf("i am child\n");
  else{
    sleep(1);
    printf("i am parent\n");
  }
  exit(0);
}
