#include <stdio.h>
#include <stdlib.h>

//全局变量没有显式初始化会自动初始化成0
int a;

int main(){
  printf("%d\n",a);

  exit(0);
}
