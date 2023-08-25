#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/resource.h>

#define BUFSIZE 1024
static char buf[BUFSIZE];

int main(){
  mode_t umask_save;
  pid_t pid,sid;
  struct rlimit l;
  int i,fd;
  //umask 设置为0
  umask_save = umask(0);

  //fork进程
  pid = fork();
  if(pid < 0){
    perror("fork()");
    exit(0);
  }else if(pid == 0){
    //创建会话
    sid = setsid();
    printf("sid = %d\n",sid);
    if(sid < 0){
      perror("setsid()");
      exit(1);
    } 
  }else{
    //父进程退出
    exit(0);
  }

  if(chdir("/") == -1){
    perror("chdir()");
    exit(1);
  }
  //打印当前工作目录，检查chdir是否生效
  printf("current working directory:%s\n",getcwd(buf,BUFSIZE));

  if(getrlimit(RLIMIT_NOFILE,&l) != 0){
    perror("getrlimit()");
    exit(1);
  }

  //关闭不必要的文件
  if(l.rlim_max == RLIM_INFINITY)
   l.rlim_max = 1024;
  for(i = 0; i < l.rlim_max; i++){
    close(i);
  }

  //标准输入输出错误重定向到 /dev/null
  fd = open("/dev/null",O_RDWR);
  if(fd < 0){
    perror("open()");
    exit(1);
  } 
  if(dup(fd) < 0){
    perror("dup()");
    exit(1);
  }
  if(dup(fd) < 0){
    perror("dup()");
    exit(1);
  }
  umask(umask_save);
  pause();
  exit(0);
}
