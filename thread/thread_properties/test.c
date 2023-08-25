#include <stdio.h>
#include <stdlib.h>


int main(){
  int a = 0x12345678;

  char b;
  b = *(&a);

  if(b == 0x12){
    printf("大端对齐\n");
  }else if(b == 0x78){
    printf("小端对齐\n");
  }else{
    printf("unknow\n");
  }

  exit(0);
}
