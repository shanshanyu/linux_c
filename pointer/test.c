#include <stdio.h>
#include <stdlib.h>

//空指针和 0 的关系

// NULL == (void *)0

int main(){

  char *p;
  p = 0;
  
  int *q;
  q = NULL;
  if(p == q) printf("p and q is null\n");

}
