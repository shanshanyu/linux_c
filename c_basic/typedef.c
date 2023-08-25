#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int b[20];

typedef struct msg_st{  //结构体是一个数据类型，不是一个变量，结构体的成员不能直接赋值  char name[20] = "tom";   是错的
  char name[20];
}msg_st;



int main(){
  b c;
  int i;
  
  msg_st  msg;
  strcpy(msg.name,"jerry");

  for(i = 0; i < 20; i++)
    c[i] = i;

  for(i = 0; i < 20; i++)
    printf("c[%d] = %d\n",i,c[i]);


  printf("msg: %s\n",msg.name);

  exit(0);
}
