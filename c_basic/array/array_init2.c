#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  int i,j;
  //int b[2][2] = {{0},{0}}; 
  //int b[2][2] = {0};
  int b[2][2];
  memset(b,0,sizeof(b));



  for(i = 0; i < 2; i++)
    for(j = 0; j < 2; j++)
      printf("%x\n",b[i][j]);

  /*for(i = 0; i < 4; i++){
    for(j = 0; j < 4; j++)
      printf("%d\n",b[i][j]);
  }*/

  exit(0);
}
