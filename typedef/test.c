#include <stdio.h>
#include <stdlib.h>

//结构体验证 ok，比如 sqlist linklist等

/*
 *   char *arg[]   指针数组
 *   char (*arg)[]  指向二维数组的指针变量
 *  char (*arg)()   函数指针
 */



typedef int a;

typedef int x[20];

int main(){
  //基本类型，验证 ok
  a b = 3;
  printf("%d\n",b);

  //构造类型,验证 ok
  x c;
  int i;
  for(i = 0;i < 20; i++)
    c[i] = i;
  for(i = 0;i < 20; i++)
    printf("%d\n",c[i]);
  
  return 0;
}
