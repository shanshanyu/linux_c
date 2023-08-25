#include <stdio.h>
#include <stdlib.h>


//测试字符数组的长度

int main(){
  int i;
  char c[]={'c', ' ','p','r','o','g','r','a','m'};

  char d[]={"C program"};  

  char e[]="C program";

  printf("%s\n",c);
  puts(c);
  printf("c = %ld\n",sizeof(c));
  printf("d = %ld\n",sizeof(d));
  printf("e = %ld\n",sizeof(e));

  exit(0);
}
