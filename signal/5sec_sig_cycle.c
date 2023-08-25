#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>

/*
 *count++   alarm  print
 *
 *
 */

static jmp_buf env;

//static int flag = 1;
//使用编译器优化会提升一个量级
static volatile int flag = 1;
void alrm_handler(int signo){
  printf("signal %d received\n",signo);
  //longjmp(env,1); 
  flag = 0; 
  alarm(5);
}

int main(){
  int64_t count = 0;
  signal(SIGALRM,alrm_handler);
  
  //信号处理函数中使用 longjmp,保证从信号处理程序返回后不执行 if 语句的代码
  /*if(setjmp(env) == 0){
    alarm(5);
    pause();
  }
  */
  alarm(5);
while(1){
  while(flag)
    count++;

  flag = 1;
  printf("count = %lld\n",count);  
}
  exit(0);
}
