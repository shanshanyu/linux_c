#include <stdio.h>
#include <stdlib.h>

//逗号运算符的优先级比赋值运算符低，要用一个括号括起来

int main(){
  int a=2,b=4,c=6,x,y;
  y=((x=a+b),(b+c));
  printf("y=%d,x=%d",y,x);

  exit(0);
}
