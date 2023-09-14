#include <stdio.h>
#include <stdlib.h>

int add(int,int);
int sub(int,int);
int mul(int,int);
int divede(int,int);

int main(){
  int a,b;

  printf("请输入 a,b: ");
  scanf("%d,%d",&a,&b);

  printf("a+b=%d\n",add(a,b));
  printf("a-b=%d\n",sub(a,b));
  printf("a*b=%d\n",mul(a,b));
  printf("a/b=%d\n",divede(a,b));
  return 0;

}
