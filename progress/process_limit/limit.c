#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

//打印进程的资源限制

int main(){
  struct rlimit l;
  if(getrlimit(RLIMIT_NOFILE,&l) != 0){
    perror("getrlimit()");
    exit(1);
  }
  printf("NOFILE soft limit: %ld\n",l.rlim_cur);
  printf("NOFILE hard limit: %ld\n",l.rlim_max);

  //将软限制值改成 60000
  l.rlim_cur = 60000;
  if(setrlimit(RLIMIT_NOFILE,&l) != 0){
    perror("setrlimit()");
    exit(1);
  }
  
  printf("NOFILE soft limit: %ld\n",l.rlim_cur);
  printf("NOFILE hard limit: %ld\n",l.rlim_max);

  exit(0);
}
