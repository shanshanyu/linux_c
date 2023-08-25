#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>

#define BUFSIZE 10

/*
 *实现 cat 的功能
 *由于第一版判断逻辑不清晰
 *需要流控，每秒钟输出 10 个字符; sleep 和 alarm 不能混用，每个程序只能有一个时钟。sleep可能就是用alarm实现的
 * 漏桶实现
 */

static int loop = 1;

void alrm_handler(int sig){
  loop = 0;
  //alarm(1);
}


int main(int argc,const char *argv[]){
  //测试参数是否合适
  if(argc != 2){
    printf("Usage:%s  old_file\n",argv[0]);
    exit(0);
  }
  //初始化 settimer 的结构体
  struct itimerval it_new,it_old;
  it_new.it_interval.tv_sec = 1;
  it_new.it_interval.tv_usec = 0;
  it_new.it_value.tv_sec = 1;
  it_new.it_value.tv_usec = 0;

  int fd_old,fd_new=1;
  char buf[BUFSIZE];
  int rd_ret,wr_ret;
  int pos = 0;

  fd_old = open(argv[1],O_RDONLY);
  if(fd_old == -1){
    printf("open old file failed, please check\n");
    exit(1);
  }

  signal(SIGALRM,alrm_handler);
  //alarm(1); 
  //用 setitimer 实现周期定时
  if(setitimer(ITIMER_REAL,&it_new,NULL) == -1){
    perror("setitimer()");
    exit(1);
  }

while(1){
  //read 可能返回 2 种值，第一种 -1，如果 errno 不等于EINTR 直接退出，如果等于，继续循环读。第二种一个整数，继续后面的写
  //循环退出条件是读到了数
  rd_ret = read(fd_old,buf,BUFSIZE);
  //read 返回值做判断
  if(rd_ret < 0){
    if(errno == EINTR) continue;
    else
      exit(1);
  }
  //如果读到文件尾直接退出循环
  if(rd_ret == 0) break; 

  //write 可能返回 2 种值，第一个种 -1，如果 errno 不等于EINTR 直接退出，如果等于，继续循环写。第二种是一个整数，如果小于 rd_ret，继续写
  //循环退出条件是写完了(没写完被信号中断继续写，没写被信号中断继续写）
  pos = 0;
  do{
    wr_ret = write(fd_new,buf,rd_ret);
    if(wr_ret < 0){
      if(errno == EINTR) continue;
      else
        exit(1);
    }
    pos += wr_ret;   
  }while(pos < rd_ret);
  //wait alrm signal
  while(loop)
    pause();  //用 pause 比用 ; 好
  loop = 1;
}
  
  exit(0);
}
