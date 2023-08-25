#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

/*
 *打印一个方块的形状，用二维数组实现
 *
 *
 */

void update_rock();


typedef struct tetris{
  int pos[4][4];
}tetris;

//定义外围边框的二维数组
int map[20][20];
//俄罗斯方块有 7 种图形，4种变形，一共 28 种图形，通过二维结构体数组来存储
tetris rock[7][4];

//图形位置
int x,y;
//图形以及变形
int s,r;

//设置光标位置
void gotoxy(int x,int y){
  printf("\033[%d;%dH",y,x);
}


//初始化图形数组，初始化二维数组 i->y  j->x
void init_map(){
  int i,j;
  for(i = 0; i < 20; i++){
    for(j= 0; j < 20; j++){
      if(i ==0 || i == 19 || j== 0 || j == 19)
        map[i][j] = 1;
      else
        map[i][j] = 0;
    }
  }
}

//显示图形
void print_map(){
  int i,j;
  for(i = 0; i < 20; i++){
    for(j = 0; j < 20; j++){
      if(map[i][j] == 1){
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
void print_square(int s,int r,int x,int y,int flag){
  int i,j;
  if(flag == 1){
    for(i = 0; i < 4; i++){
      for(j = 0; j < 4; j++)
        if(rock[s][r].pos[i][j] == 1){
          gotoxy(2*(j+x)+1,i+1+y);
          printf("[]");
        }
      printf("\n");
    }
  }else{
    for(i = 0; i < 4; i++){
      for(j = 0; j < 4; j++)
        if(rock[s][r].pos[i][j] == 1){
          gotoxy(2*(j+x)+1,i+1+y);
          printf("  ");
        }
      printf("\n");
    }
  }
}

void init_rock(){
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


//判断方块是否到达底部
int touch_buttom(int s,int r,int x,int y){
  int i,j;
  int flag = 0;
  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      if(rock[s][r].pos[i][j] == 1)
        if(map[y+i][x+j] == 1){
          flag = 1;
          break;
        }
      
    
  
  return flag;
}

//play_game负责一个图形的移动，移动到底部结束，play_game 也返回了
void play_game(){
  int i,j;
  s = rand() % 7;
  r = rand() % 4;
  x = 8;
  y = 1;
  
  //画出边框
  print_map(map);


  while(1){
    update_rock();
    //方块再下落一次到达底部
    if(touch_buttom(s,r,x,y+1)){
      for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++){
          //把图形固定到map上去
          if(rock[s][r].pos[i][j] == 1){
            map[i+y][j+x] = 1;
          }
        }
      break;
    }else{
      print_square(s,r,x,y,1);
      sleep(1);
      print_square(s,r,x,y,0);
      y++;
    }
  } 

}

//通过键盘输入 'a' 's' 'w' 'd' 改变图形的位置
void update_rock(){
  int ch;
  ch = getchar();
  switch(ch){
    //旋转图形，顺时针旋转
    case 'w':
      if(!touch_buttom(s,(r+1)%4,x,y)){
        print_square(s,r,x,y,0);
        r = (r+1)%4;
        print_square(s,r,x,y,1);
      }
      break;
    case 'a':
      if(!touch_buttom(s,r,x-1,y)){
        print_square(s,r,x,y,0);
        x--;
        print_square(s,r,x,y,0);
      }
      break;
    case 's':
      if(!touch_buttom(s,r,x,y+1)){
        print_square(s,r,x,y,0);
        y++;
        print_square(s,r,x,y,1);
      }
      break;
    case 'd':
      if(!touch_buttom(s,r,x+1,y)){
        print_square(s,r,x,y,0);
        x++;
        print_square(s,r,x,y,1);
      }
      break;
  }
}



int main(){
  struct termios term,save_term;
  
  //初始化map 和 rock
  init_map();
  init_rock();
  //清屏
  printf("\033[2J]");
  
  //获取、设置终端属性
  if(tcgetattr(STDIN_FILENO,&term) < 0){
    perror("tcgetattr()");
    exit(1);
  }
  save_term = term;

  //关闭回显和规范模式
  term.c_lflag &= ~(ECHO|ICANON); 
  term.c_cc[VTIME] = 0;
  term.c_cc[VMIN] = 1;

  if(tcsetattr(STDIN_FILENO,TCSANOW,&term) < 0){
    perror("tcsetattr()");
    exit(1);
  }

 while(1){
    play_game();
  }

  //恢复终端属性
  if(tcsetattr(STDIN_FILENO,TCSANOW,&save_term) < 0){
    perror("tcsetattr()");
    exit(1);
  }

 exit(0);
}
