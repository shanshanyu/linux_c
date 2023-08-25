#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//程序结果：hellojerryjerry
//buf 在静态区上，一个程序多次调用 test_static，后面调用的会冲洗前面调用的


char * test_static(const char *str){
  static char buf[1024];
  strcpy(buf,str);
  return buf;
}


int main(){
  char *buf,*buf1;
  buf = test_static("hello");
  fputs(buf,stderr);
   
  buf1 = test_static("jerry");
  fputs(buf1,stderr);
  fputs(buf,stderr); 

  exit(0);
}
