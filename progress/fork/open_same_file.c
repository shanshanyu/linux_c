#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

//子进程通过父进程复制过来的文件描述符，它们的文件表项关系?
//通过这个程序可以得出来一个结论：父子进程的文件表项是共享的，如果父进程修改了，子进程也会感知到   本例的结果是：worldhello

int main(){
  int fd;
  pid_t pid;
  fd = open("/tmp/test",O_RDWR|O_CREAT|O_TRUNC,0644);
  if(fd < 0){
    perror("open()");
    exit(1);
  }

  pid = fork();
  if(pid < 0){
    perror("fork()");
  }else if(pid == 0){
    write(fd,"hello",5);
    exit(0);
  }else{
    write(fd,"world",5);
  }
  wait(NULL);
  close(fd);
  exit(0);
}
