#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//查看每秒钟的时钟滴答数

//不同平台的滴答数不一样，i386平台是 100 次，每10ms时钟滴答一次，每秒钟时钟滴答100次

int main(){
  printf("dida num:%ld\n",sysconf(_SC_CLK_TCK));

  exit(0);
}
