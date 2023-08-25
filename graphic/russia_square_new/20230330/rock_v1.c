#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 *打印一个方块的形状，用二维数组实现
 *
 *
 */

//设置光标位置
void gotoxy(int x,int y){
  printf("\033[%d;%dH",y,x);
}


//初始化图形数组，初始化二维数组 i->y  j->x
void init_map(int (*s)[20]){
  int i,j;
  for(i = 0; i < 20; i++){
    for(j= 0; j < 20; j++){
      if(i ==0 || i == 19 || j== 0 || j == 19)
        s[i][j] = 1;
      else
        s[i][j] = 0;
    }
  }
}

//显示图形
void draw_map(int (*s)[20]){
  int i,j;
  for(i = 0; i < 20; i++){
    for(j = 0; j < 20; j++){
      if(s[i][j] == 1){
        gotoxy(2*j+1,i+1);
        printf("[]");
      }else{
        gotoxy(2*j+1,i+1);
        printf("  ");
      }     
    }
    printf("\n");
  }
}


//显示方块
void print_square(int (*s)[4],int x,int y,int flag){
  int i,j;
  if(flag == 1){
    for(i = 0; i < 4; i++){
      for(j = 0; j < 4; j++)
        if(s[i][j] == 1){
          gotoxy(2*(j+x)+1,i+1+y);
          printf("[]");
        }else{
          gotoxy(2*(j+x)+1,i+1+y);
          printf("  ");
        }
      printf("\n");
    }
  }else{
    for(i = 0; i < 4; i++){
      for(j = 0; j < 4; j++)
        if(s[i][j] == 1){
          gotoxy(2*(j+x)+1,i+1+y);
          printf("  ");
        }
      printf("\n");
    }
  }
}



int main(){
  int x,y;
  int s[4][4];
  int map[20][20];
  
  x = 1;
  y = 1;
  memset(s,0,sizeof(s));
  s[0][1] = 1;
  s[1][0] = 1;
  s[1][1] = 1;
  s[1][2] = 1;  
 
  //初始化二维数组
  init_map(map);
  
  //清屏
  printf("\033[2J]");

  //通过二维数组画图
  draw_map(map);

  while(1){
    print_square(s,x,y,1);
    sleep(1);
    print_square(s,x,y,0);
    y++;
  } 

  exit(0);
}
