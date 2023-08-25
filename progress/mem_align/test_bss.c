#include <stdio.h>
#include <stdlib.h>

int a;
static int b;


int c = 1;
static int d = 2;
int main(){
  int e;
  static int f;
  printf("a=%d\n",a);
  printf("b=%d\n",b);
  printf("a address=%p\n",&a);
  printf("b address=%p\n",&b);

  printf("c address = %p\n",&c);
  printf("d address = %p\n",&d);
  printf("e address = %p\n",&e);
  printf("f address = %p\n",&f);
  return(0);


}
