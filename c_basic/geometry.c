#include <stdio.h>
#include <stdlib.h>

//打印三角形
void print_triangle(int n){
  int i,j,k;
  for(i = 1; i <= n; i += 2){
    for(j = 1; j <= (n-i)/2; j++) //打印空格
      printf(" "); //打印空格
    for(k = 1; k <= i; k++) //打印星号
      printf("*");
    printf("\n");
  }
}


int main(){

  print_triangle(50);

  exit(0);
}
