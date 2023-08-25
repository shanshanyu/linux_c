#include <stdio.h>
#include <stdlib.h>

int main(){
  int *ptr;
  int a = 3;
  ptr = &a;

  printf("%p\n",ptr);
  printf("%p\n",ptr+1);

  exit(0);
}
