#include <stdio.h>
#include <stdlib.h>

static int c;

int main(){
  //具有空链接的静态，不赋值初始值是什么？
  //静态变量
  static int a;
  int b;
  printf("%d\n",a);
  printf("%d\n",b);
  printf("%d\n",c);
  exit(0);
}
