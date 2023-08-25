#include <stdio.h>
#include <stdlib.h>

//交换 a b 的值，不使用额外的变量

int main(){
  int a = 3;
  int b = 4;

  printf("a = %d\n",a);
  printf("b = %d\n",b);

  a = a ^ b;
  b = a ^ b; //b = a
  a = a ^ b; //a = b;

  printf("a = %d\n",a);
  printf("b = %d\n",b);

  exit(0);
}
