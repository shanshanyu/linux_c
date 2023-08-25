#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define MEMSIZE 1024

int main(){
  pid_t pid;
  char *ptr;
  //匿名存储映射
  ptr = mmap(NULL,MEMSIZE,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
  if(ptr == MAP_FAILED){
    perror("mmap()");
    exit(1);
  }

  pid = fork();
  if(pid < 0){
    perror("fork()");
    //如果进程fork失败，释放之前映射的存储区
    munmap(ptr,MEMSIZE);
    exit(1);
  }else if(pid == 0){
  //child
    strcpy(ptr,"hello world\n");
  }else{
   //parent
   wait(NULL);
   puts(ptr); //puts 会加一个换行输出
   
  }
  munmap(ptr,MEMSIZE);
  exit(0);
}
