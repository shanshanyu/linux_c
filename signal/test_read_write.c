#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>


/*
 * 测试 read 在各种情况下的返回值
 *读到字符，返回读到的字符。如果读到的字符小于需要读的字符，那么返回值是什么？
 *
 */

/*
 *read return val:
 * 读到的字节数（读到的字节数等于需要的字节数）
 * 读到的字节数（读到的字节数小于需要的字节数：被信号打断、到文件末尾、读管道、读终端）
 * 0（读到尾部再次读会返回 0)
 * -1(出错） 还需要判断一次是不是 EINTR 假错现象
 */

/*
 *write return val:
 *返回成功写入的字节数(写入的字节数等于需要的字节数）
 *返回成功写入的字节数（写入的字节数小于需要的字节数）
 *0(只有count是0的时候会出现）
 *-1（出错）还需要判断一次是不是 EINTR 假错现象
 */

#define BUFSIZE 100
int main(){
  char buf[BUFSIZE];
  int ret;
  //捕捉信号
 // signal(SIGINT,int_handler);
  int fd,fd1;
  fd = open("/etc/services",O_RDONLY);
  fd1 = open("/tmp/out",O_WRONLY|O_CREAT|O_TRUNC,0644);
  ret = read(fd,buf,BUFSIZE);
  printf("read ret = %d\n",ret);
  ret = write(fd1,buf,0);
  printf("write ret = %d\n",ret);
  perror(""); 
  exit(0);
}
