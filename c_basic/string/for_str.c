#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//逆序遍历字符串

int main(){
  char s[] = "abcd";

  int len;
  len = strlen(s) - 1;
  while(len >= 0){
    printf("%c",s[len]);
    len--;
  }

  exit(0);
}
