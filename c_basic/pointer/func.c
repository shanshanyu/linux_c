#include <stdio.h>
#include <stdlib.h>

void swap(int *a,int *b){
  a = NULL;
  b = NULL;

}

int main(){
  int a=3,b=4;
  swap(&a,&b);
  printf("a = %d\n",a);
  printf("b = %d\n",b);
  exit(0);
}
