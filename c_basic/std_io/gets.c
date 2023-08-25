#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//gets 读到的数组包括换行吗？
//不包括


int main(){
  char str[1];
  //fgets(str,5,stdin);
  gets(str);
  //puts(str);
  printf("%s",str);
  printf("strlen = %ld\n",strlen(str));
  exit(0);
}
