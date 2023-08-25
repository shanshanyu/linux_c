#include <stdio.h>
#include <stdlib.h>

int main(){
  int ch;
  //printf("L_ctermid = %d",L_ctermid);
  //输入一个字符，然后打印字符的 ascii 码
  //输入一个 b 按回车，第一次 printf  打印 b  接着打印回车的 ascii 码，也就是 a
  while(1){
    ch = getchar();
    printf("%x",ch);
    //fflush(NULL);
    //putchar(ch);
  }

  exit(0);
}
