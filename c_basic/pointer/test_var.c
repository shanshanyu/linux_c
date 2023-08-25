#include <stdio.h>
#include <stdlib.h>


//测试指针常量的加法运算
//测试结果：和指针变量是一样的

int main(){
  int a;
  int *p = &a+1;

  char b;
  char *p1 = &b+1;


  printf("%p\n",&a);
  printf("%p\n",p);
  printf("sizeof(int) = %ld\n",sizeof(int));

  printf("%p\n",&b);
  printf("%p\n",p1);
  printf("sizeof(char) = %ld\n",sizeof(char));

  exit(0);
}
