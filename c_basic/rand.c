#include <stdio.h>
#include <stdlib.h>


//测试 rand 和 srand
//程序启动第一次调用 rand 的值是不是相同的？  是相同的
//用srand 参数为 1 产生的种子，接着调用 rand 产生的随机数是不是和程序第一次启动调用rand的序列相同？  是相同的

int main(){
  srand(1);
  printf("rand = %d\n",rand());
  printf("rand = %d\n",rand());
  printf("rand = %d\n",rand());
  exit(0);
}
