#include <stdio.h>
#include <stdlib.h>
#define COLUMN_LEN 60
#define ROW_LEN 30

//打印一个 60行宽，30列长的长方形
//1,1  60,1   1,30  60,30 是四个角的坐标   内容大小 28,58 

int main(){
  int i,j;
  printf("\033[2J");
  printf("\033[1;1H");
  //打印上行
  for(i = 0; i < COLUMN_LEN; i++) 
    printf("\033[35;42m=\033[0m");

  printf("\033[30;1H");

  //打印下行
  for(i = 0; i < COLUMN_LEN; i++)
    printf("\033[35;42m=\033[0m");

  for(j = 2; j < ROW_LEN; j++){
    printf("\033[%d;1H",j);
    printf("\033[35;42m|\033[0m");
    printf("\033[%d;60H",j);
    printf("\033[35;42m|\033[0m");
  
  }
  printf("\033[31;1H");
  exit(0);
}
