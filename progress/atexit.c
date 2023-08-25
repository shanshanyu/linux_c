#include <stdio.h>
#include <stdlib.h>

void cleanup1(){
  printf("cleanup1 running\n");
}

void cleanup2(){
  printf("cleanup2 running\n");
}

int main(){
  int ret;
  ret = atexit(cleanup1);
  ret = atexit(cleanup2);  

  return 0;
}
