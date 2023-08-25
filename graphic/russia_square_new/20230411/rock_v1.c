#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 *打印一个方块的形状，用二维数组实现
 *
 *
 */

typedef struct tetris{
  int pos[4][4];
}tetris;

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
void print_map(int (*s)[20]){
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


//显示方块,在指定位置显式指定图形  s 指定图形，r指定变形
void print_square(tetris (*t)[4],int s,int r,int x,int y,int flag){
  int i,j;
  if(flag == 1){
    for(i = 0; i < 4; i++){
      for(j = 0; j < 4; j++)
        if(t[s][r].pos[i][j] == 1){
          gotoxy(2*(j+x)+1,i+1+y);
          printf("[]");
        }
      printf("\n");
    }
  }else{
    for(i = 0; i < 4; i++){
      for(j = 0; j < 4; j++)
        if(t[s][r].pos[i][j] == 1){
          gotoxy(2*(j+x)+1,i+1+y);
          printf("  ");
        }
      printf("\n");
    }
  }
}

void init_rock(tetris (*rock)[4]){
  //第一种图形的 4 种变形
  rock[0][0].pos[0][1] = 1;
  rock[0][0].pos[1][0] = 1;
  rock[0][0].pos[1][1] = 1;
  rock[0][0].pos[1][2] = 1;

  rock[0][1].pos[0][0] = 1;
  rock[0][1].pos[1][0] = 1;
  rock[0][1].pos[1][1] = 1;
  rock[0][1].pos[2][0] = 1;

  rock[0][2].pos[0][0] = 1;
  rock[0][2].pos[0][1] = 1;
  rock[0][2].pos[0][2] = 1;
  rock[0][2].pos[1][1] = 1;

  rock[0][3].pos[0][1] = 1;
  rock[0][3].pos[1][0] = 1;
  rock[0][3].pos[1][1] = 1;
  rock[0][3].pos[2][1] = 1;

  //第二种图形的 4 种变形
  rock[1][0].pos[0][0] = 1;
  rock[1][0].pos[0][1] = 1;
  rock[1][0].pos[1][1] = 1;
  rock[1][0].pos[1][2] = 1;

  rock[1][1].pos[0][1] = 1;
  rock[1][1].pos[1][0] = 1;
  rock[1][1].pos[1][1] = 1;
  rock[1][1].pos[2][0] = 1;

  rock[1][2].pos[0][0] = 1;
  rock[1][2].pos[0][1] = 1;
  rock[1][2].pos[1][1] = 1;
  rock[1][2].pos[1][2] = 1;

  rock[1][3].pos[0][1] = 1;
  rock[1][3].pos[1][0] = 1;
  rock[1][3].pos[1][1] = 1;
  rock[1][3].pos[2][0] = 1;

  //第三种图形的 4 种变形
  rock[2][0].pos[0][0] = 1;
  rock[2][0].pos[1][0] = 1;
  rock[2][0].pos[2][0] = 1;
  rock[2][0].pos[2][1] = 1;

  rock[2][1].pos[0][0] = 1;
  rock[2][1].pos[0][1] = 1;
  rock[2][1].pos[0][2] = 1;
  rock[2][1].pos[1][0] = 1;

  rock[2][2].pos[0][0] = 1;
  rock[2][2].pos[0][1] = 1;
  rock[2][2].pos[1][1] = 1;
  rock[2][2].pos[2][1] = 1;

  rock[2][3].pos[0][2] = 1;
  rock[2][3].pos[1][0] = 1;
  rock[2][3].pos[1][1] = 1;
  rock[2][3].pos[1][2] = 1;


  //第四种图形的 4 种变形
  rock[3][0].pos[0][1] = 1;
  rock[3][0].pos[0][2] = 1;
  rock[3][0].pos[1][0] = 1;
  rock[3][0].pos[1][1] = 1;

  rock[3][1].pos[0][0] = 1;
  rock[3][1].pos[1][0] = 1;
  rock[3][1].pos[1][1] = 1;
  rock[3][1].pos[2][1] = 1;

  rock[3][2].pos[0][1] = 1;
  rock[3][2].pos[0][2] = 1;
  rock[3][2].pos[1][0] = 1;
  rock[3][2].pos[1][1] = 1;

  rock[3][3].pos[0][0] = 1;
  rock[3][3].pos[1][0] = 1;
  rock[3][3].pos[1][1] = 1;
  rock[3][3].pos[2][1] = 1;

  //第五种图形的 4 种变形
  rock[4][0].pos[0][1] = 1;
  rock[4][0].pos[1][1] = 1;
  rock[4][0].pos[2][0] = 1;
  rock[4][0].pos[2][1] = 1;

  rock[4][1].pos[0][0] = 1;
  rock[4][1].pos[1][0] = 1;
  rock[4][1].pos[1][1] = 1;
  rock[4][1].pos[1][2] = 1;

  rock[4][2].pos[0][0] = 1;
  rock[4][2].pos[0][1] = 1;
  rock[4][2].pos[1][0] = 1;
  rock[4][2].pos[2][0] = 1;

  rock[4][3].pos[0][0] = 1;
  rock[4][3].pos[0][1] = 1;
  rock[4][3].pos[0][2] = 1;
  rock[4][3].pos[1][2] = 1;

  //第六种图形的 4种变形
  rock[5][0].pos[0][0] = 1;
  rock[5][0].pos[1][0] = 1;
  rock[5][0].pos[2][0] = 1;
  rock[5][0].pos[3][0] = 1;

  rock[5][1].pos[0][0] = 1;
  rock[5][1].pos[0][1] = 1;
  rock[5][1].pos[0][2] = 1;
  rock[5][1].pos[0][3] = 1;

  rock[5][2].pos[0][0] = 1;
  rock[5][2].pos[1][0] = 1;
  rock[5][2].pos[2][0] = 1;
  rock[5][2].pos[3][0] = 1;

  rock[5][3].pos[0][0] = 1;
  rock[5][3].pos[0][1] = 1;
  rock[5][3].pos[0][2] = 1;
  rock[5][3].pos[0][3] = 1;

  //第七种图形的 4 种变形
  rock[6][0].pos[0][0] = 1;
  rock[6][0].pos[0][1] = 1;
  rock[6][0].pos[1][0] = 1;
  rock[6][0].pos[1][1] = 1;

  rock[6][1].pos[0][0] = 1;
  rock[6][1].pos[0][1] = 1;
  rock[6][1].pos[1][0] = 1;
  rock[6][1].pos[1][1] = 1;

  rock[6][2].pos[0][0] = 1;
  rock[6][2].pos[0][1] = 1;
  rock[6][2].pos[1][0] = 1;
  rock[6][2].pos[1][1] = 1;

  rock[6][3].pos[0][0] = 1;
  rock[6][3].pos[0][1] = 1;
  rock[6][3].pos[1][0] = 1;
  rock[6][3].pos[1][1] = 1;

}



int main(){
  int x,y;
  //控制打印哪种图形，哪种变形
  int s,r;
  int map[20][20];
  //俄罗斯方块有 7 种图形，4种变形，一共 28 种图形，通过二维结构体数组来存储
  tetris t[7][4]; 
  //初始化二维数组
  memset(t,0,sizeof(t));
  

  x = 1;
  y = 1;
  //初始化二维数组
  init_map(map);
  init_rock(t);
  
  //清屏
  printf("\033[2J]");

  //通过二维数组画图
  print_map(map);
  s = rand() % 7;
  r = rand() % 4;

  printf("s = %d,r = %d\n",s,r);

  while(1){
    print_square(t,s,r,x,y,1);
    sleep(1);
    print_square(t,s,r,x,y,0);
    y++;
  } 

  exit(0);
}
