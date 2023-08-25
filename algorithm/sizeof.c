#include <stdio.h>
#include <stdlib.h>

//测试数组的 sizeof 和指针的 sizeof


void test_func(int s[]){   //这个s是一个指针地址
  printf("sizeof 函数参数s:%ld\n",sizeof(s));
}


int main(){
  int a[] = {1,3,4};  
  int *p = a;
  printf("数组a的sizeof为:%ld,指针的 sizeof 为: %ld\n",sizeof(a),sizeof(p));  //a是数组的长度，p是指针的长度
  test_func(a);

  exit(0);
}
