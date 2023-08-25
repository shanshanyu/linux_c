#include <stdio.h>
#include <stdlib.h>

//可以访问越界数组，数据未知，如果没有权限访问会报错

int main(){
  char str[5];
  int i;
  int a[5] = { 1,2,3,4};

  gets(str);

  for(i = 0; i < 10; i++)
    putchar(str[i]);


  for(i = 0; i < 10; i++)
    printf("%d\n",a[i]);


  exit(0);
}
