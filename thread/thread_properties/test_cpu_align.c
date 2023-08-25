#include <stdio.h>
#include <stdlib.h>

int main(){
  //数据存放在栈中,向下增长,a的地址是高位地址
  //0x12345678,12 是高位，78 是低位
  int a = 0x12345678;
  //printf("%d\n",&a);
  char *c = (char *)(&a);
  printf("%x\n",*c); 
  if(*c == 0x12)
    printf("大端对齐\n");
  else if(*c == 0x78)
    printf("小端对齐\n");
  else
    printf("error\n");

  int *b = malloc(sizeof(int));
  *b = 0x12345678;
  char *d = (char *)b;
  printf("%x\n",*d);
  if(*d == 0x12)
    printf("大端对齐\n");
  else if(*d == 0x78)
    printf("小端对齐\n");
  else
    printf("error\n");
  return 0;
}
