#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//子进程中修改 自动变量   全局变量是否影响父进程？
//子进程不会改变父进程空间的任何变量


int global_var = 3;

int main(){
  int local_var = 1;
  pid_t pid;
  pid = fork();
  if(pid < 0){
    perror("fork()");
  }else if(pid == 0){
    global_var++;
    local_var++;
    exit(0);
  }else{
    sleep(2);
  }

  printf("pid = %d,global_var = %d,local_var = %d\n",getpid(),global_var,local_var);
  exit(0);
}
