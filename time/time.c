#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define BUFSIZE 1024

/*
 *time function:
 *time    time_t
 *localtime   gmtime    struct tm
 *clock_gettime   clock_settime   struct timespec
 *gettimeofday    struct timeval
 *strftime strftime_1
 */

int main(){
  time_t t;
  //返回日历时间
  t = time(NULL);
  printf("time func now: %d\n",t);

  //gettimeofday 会获得一个 timeval 结构，里面包含 tv_sec 
  struct timeval t1;
  gettimeofday(&t1,NULL);
  printf("gettimeofday func now:%d\n",t1.tv_sec);
  printf("gettimeofday func now:%ld\n",t1.tv_usec);
  
  //clock_gettime 会获得一个 timespec 结构，里面包含 tv_sec
  struct timespec t2;
  clock_gettime(CLOCK_REALTIME,&t2);
  printf("clock_gettime func now:%d\n",t2.tv_sec);
  printf("clock_gettime func now:%ld\n",t2.tv_nsec);

  //日历时间转换成分解时间，结构体是 struct tm
  struct tm *t3;
  t3 = localtime(&t);
  printf("localtime func now:%d%d%d\n",t3->tm_year+1900,t3->tm_mon,t3->tm_mday);

  //分解时间转换成日历时间，把本地时间转换成日历时间，（不是协调时间）
  time_t t_1;
  t_1 = mktime(t3);
  printf("mktime func now:%d\n",t_1);
 
  //把分解时间转换成字符串
  char buf[BUFSIZE];
  strftime(buf,BUFSIZE,"%Y%m%d %H%M%S",t3);
  printf("strftime func now:%s\n",buf);  

  return 0;
}
