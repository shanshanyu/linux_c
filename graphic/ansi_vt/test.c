#include <stdio.h>
#include <stdlib.h>

//测试printf 的各种控制符

int main(){
  int a = -10;
  int b = 033;
  float c = 3.1415926;
  printf("a = %+5d\n",a);
  printf("a = %u\n",a);
  printf("b = %+d\n",b);
  printf("b = %#o\n",b);
  printf("c = %.2f\n",c);
  exit(0);
}
