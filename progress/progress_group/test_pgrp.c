#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 *bash 创建的子进程和 bash 进程不属于同一进程组
 *
 *一个进程 fork/exec 后，子进程和父进程属于同一进程组
 *
 *应该是 bash 创建子进程调用 exec 之前调用了 setpgid，将子进程从自己的进程组中独立出来
 *
 */


int main(){
  pid_t pid;
  pid = fork();
  if(pid < 0){
    perror("fork()");
    exit(1);
  }else if(pid == 0){
    if(setpgid(0,0) == -1){
      perror("setpgid()");
      exit(1);
    }
    execl("/bin/cat","cat",NULL);
  }else{
    wait(NULL);

  }

  exit(0);
}
