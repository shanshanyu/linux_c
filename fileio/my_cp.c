#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFSIZE 1024

/*
 *实现 cp 的功能
 *
 */



int main(int argc,const char *argv[]){
  //测试参数是否合适
  if(argc != 3){
    printf("Usage:%s  old_file new_file\n",argv[0]);
    exit(0);
  }

  int fd_old,fd_new;
  char buf[BUFSIZE];
  int rd_ret,wr_ret;
  int pos = 0;

  fd_old = open(argv[1],O_RDONLY);
  if(fd_old == -1){
    printf("open old file failed, please check\n");
    exit(1);
  }

  fd_new = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0644);
  if(fd_new == -1){
    printf("open new file failed, please check\n");
    exit(1);
  }

//判断逻辑比较混乱
do{
  //read 可能返回 2 种值，第一种 -1，如果 errno 不等于EINTR 直接退出，如果等于，继续循环读。第二种一个整数，继续后面的写
  //循环退出条件是读到了数
  //这种写法不好，0 和 大于 0 都退出循环，write 还得处理等于 0 的情况
  do{
    rd_ret = read(fd_old,buf,BUFSIZE);
    if(rd_ret == -1 && errno != EINTR){
      printf("read failed\n");
      exit(1);
    }
  }while(rd_ret == -1);

  //write 可能返回 2 种值，第一个种 -1，如果 errno 不等于EINTR 直接退出，如果等于，继续循环写。第二种是一个整数，如果小于 rd_ret，继续写
  //循环退出条件是写完了(没写完被信号中断继续写，没写被信号中断继续写）
  //write 这个写法有问题
  //如果 wr_ret == -1 但是 errno== EINTR就有问题
  pos = 0;
  do{
    wr_ret = write(fd_new,buf,rd_ret);
    if(wr_ret == -1 && errno != EINTR){
      printf("write failed\n");
      exit(1);
    }
    pos += wr_ret;   
  }while(pos < rd_ret);
}while(rd_ret != 0);

  exit(0);
}