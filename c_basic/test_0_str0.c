#include <stdio.h>
#include <stdlib.h>

//测试 0 和字符串 '\0' 是否相等

//相等，在进行比较的时候会通过  自动类型转换 把 '\0' 转换成 int 类型

int main(){
  if( 0 == '\0')
    printf("true");
  else
    printf("false");

  exit(0);
}

