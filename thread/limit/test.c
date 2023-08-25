#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
  printf("线程退出时操作系统试图销毁线程特定数据的最大次数：%ld\n",sysconf(_SC_THREAD_DESTRUCTOR_ITERATIONS));
  printf("进程可以创建的键的最大数目：%ld\n",sysconf(_SC_THREAD_KEYS_MAX));
  printf("一个线程栈可用的最小字节数: %ld\n",sysconf(_SC_THREAD_STACK_MIN));
  printf("进程可以创建的最大线程数: %ld\n",sysconf(_SC_THREAD_THREADS_MAX));
  printf("sc_thread_stacksize: %ld\n",sysconf(_SC_THREAD_ATTR_STACKADDR));

  exit(0);
}
