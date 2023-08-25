#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "mytbf.h"

#define BUFSIZE 1024
//令牌桶上限
//#define BURST 100


/*
 *实现 cat 的功能
 *由于第一版判断逻辑不清晰
 *需要流控，每秒钟输出 10 个字符; sleep 和 alarm 不能混用，每个程序只能有一个时钟。sleep可能就是用alarm实现的
 */





int main(int argc,const char *argv[]){
  //测试参数是否合适
  if(argc != 2){
    printf("Usage:%s  old_file\n",argv[0]);
    exit(0);
  }

  int fd_old,fd_new=1;
  char buf[BUFSIZE];
  int rd_ret,wr_ret;
  int pos = 0;
  int size;

  fd_old = open(argv[1],O_RDONLY);
  if(fd_old == -1){
    printf("open old file failed, please check\n");
    exit(1);
  }

  /*signal(SIGALRM,alrm_handler);
  alarm(1);*/

  //初始化令牌桶
  mytbf_t *bucket = mytbf_init(10,1000);
  if(bucket == NULL){
    fprintf(stderr,"mytbf_init() err\n");
    exit(1);
  }
  printf("bucket ok\n");
 
while(1){
  //read 可能返回 2 种值，第一种 -1，如果 errno 不等于EINTR 直接退出，如果等于，继续循环读。第二种一个整数，继续后面的写

  size = mytbf_fetchtoken(bucket,10); 
  if(size < 0){
    fprintf(stderr,"mytbf_fetchtoken() failed\n");
    exit(1);
  } 

  while((rd_ret = read(fd_old,buf,size)) < 0){
    //read 返回值做判断
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
  //如果取出的 token 没用掉
  if(size - rd_ret > 0)
    printf("returntoken = %d\n",mytbf_returntoken(bucket,size-rd_ret));
}
  //销毁令牌桶
  mytbf_destroy(bucket);
  exit(0);
}
