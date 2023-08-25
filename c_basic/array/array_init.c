#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  int i;
  int a[5];
  char b = 1;
  printf("%d\n",b);
  printf("%ld\n",sizeof(a));
  memset(a,1,sizeof(a));
  //int b[4][4] = {0}; 



  for(i = 0; i < 5; i++)
    printf("%x\n",a[i]);

  /*for(i = 0; i < 4; i++){
    for(j = 0; j < 4; j++)
      printf("%d\n",b[i][j]);
  }*/

  exit(0);
}
