#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

//打印一个 60行宽，30列长的长方形
//1,1  60,1   1,30  60,30 是四个角的坐标   内容大小 28,58 

static int x = 2;
static int y = 10;

struct tetris{
  int pos[4][4];
};

void  print_square(){
  int i;
  printf("\033[2J");

  //打印左右边框，1，2   33，34  是边框
  //1，22 是边框  
  //长 20 宽 30 的长方形
  for(i = 1; i <= 22; i++){
    printf("\033[%d;1H",i);
    printf("\033[35;42m[\033[0m");
    printf("\033[35;42m]\033[0m");
    printf("\033[%d;33H",i);
    printf("\033[35;42m[\033[0m");
    printf("\033[35;42m]\033[0m");
  }

  printf("\033[1;3H");
  for(i = 0; i < 30; i++){
    printf("\033[35;42m=\033[0m"); 
  }

  printf("\033[22;3H");
  for(i = 0; i < 30; i++){
    printf("\033[35;42m=\033[0m");
  }  
 
  printf("\033[23;1H");
}

void init_rock(struct tetris (*p)[4]){
  //第一种图形的 4 种变形
  p[0][0].pos[0][1] = 1;
  p[0][0].pos[1][0] = 1;
  p[0][0].pos[1][1] = 1;
  p[0][0].pos[1][2] = 1;

  p[0][1].pos[0][0] = 1;
  p[0][1].pos[1][0] = 1;
  p[0][1].pos[1][1] = 1;
  p[0][1].pos[2][0] = 1;

  p[0][2].pos[0][0] = 1;
  p[0][2].pos[0][1] = 1;
  p[0][2].pos[0][2] = 1;
  p[0][2].pos[1][1] = 1;

  p[0][3].pos[0][1] = 1;
  p[0][3].pos[1][0] = 1;
  p[0][3].pos[1][1] = 1;
  p[0][3].pos[2][1] = 1;

  //第二种图形的 4 种变形
  p[1][0].pos[0][0] = 1;
  p[1][0].pos[0][1] = 1;
  p[1][0].pos[1][1] = 1;
  p[1][0].pos[1][2] = 1;

  p[1][1].pos[0][1] = 1;
  p[1][1].pos[1][0] = 1;
  p[1][1].pos[1][1] = 1;
  p[1][1].pos[2][0] = 1;

  p[1][2].pos[0][0] = 1;
  p[1][2].pos[0][1] = 1;
  p[1][2].pos[1][1] = 1;
  p[1][2].pos[1][2] = 1;

  p[1][3].pos[0][1] = 1;
  p[1][3].pos[1][0] = 1;
  p[1][3].pos[1][1] = 1;
  p[1][3].pos[2][0] = 1;

  //第三种图形的 4 种变形
  p[2][0].pos[0][0] = 1;
  p[2][0].pos[1][0] = 1;
  p[2][0].pos[2][0] = 1;
  p[2][0].pos[2][1] = 1;

  p[2][1].pos[0][0] = 1;
  p[2][1].pos[0][1] = 1;
  p[2][1].pos[0][2] = 1;
  p[2][1].pos[1][0] = 1;

  p[2][2].pos[0][0] = 1;
  p[2][2].pos[0][1] = 1;
  p[2][2].pos[1][1] = 1;
  p[2][2].pos[2][1] = 1;

  p[2][3].pos[0][2] = 1;
  p[2][3].pos[1][0] = 1;
  p[2][3].pos[1][1] = 1;
  p[2][3].pos[1][2] = 1;


  //第四种图形的 4 种变形
  p[3][0].pos[0][1] = 1;
  p[3][0].pos[0][2] = 1;
  p[3][0].pos[1][0] = 1;
  p[3][0].pos[1][1] = 1;

  p[3][1].pos[0][0] = 1;
  p[3][1].pos[1][0] = 1;
  p[3][1].pos[1][1] = 1;
  p[3][1].pos[2][1] = 1;

  p[3][2].pos[0][1] = 1;
  p[3][2].pos[0][2] = 1;
  p[3][2].pos[1][0] = 1;
  p[3][2].pos[1][1] = 1;

  p[3][3].pos[0][0] = 1;
  p[3][3].pos[1][0] = 1;
  p[3][3].pos[1][1] = 1;
  p[3][3].pos[2][1] = 1;

  //第五种图形的 4 种变形
  p[4][0].pos[0][1] = 1;
  p[4][0].pos[1][1] = 1;
  p[4][0].pos[2][0] = 1;
  p[4][0].pos[2][1] = 1;

  p[4][1].pos[0][0] = 1;
  p[4][1].pos[1][0] = 1;
  p[4][1].pos[1][1] = 1;
  p[4][1].pos[1][2] = 1;

  p[4][2].pos[0][0] = 1;
  p[4][2].pos[0][1] = 1;
  p[4][2].pos[1][0] = 1;
  p[4][2].pos[2][0] = 1;

  p[4][3].pos[0][0] = 1;
  p[4][3].pos[0][1] = 1;
  p[4][3].pos[0][2] = 1;
  p[4][3].pos[1][2] = 1;

  //第六种图形的 4种变形
  p[5][0].pos[0][0] = 1;
  p[5][0].pos[1][0] = 1;
  p[5][0].pos[2][0] = 1;
  p[5][0].pos[3][0] = 1;

  p[5][1].pos[0][0] = 1;
  p[5][1].pos[0][1] = 1;
  p[5][1].pos[0][2] = 1;
  p[5][1].pos[0][3] = 1;

  p[5][2].pos[0][0] = 1;
  p[5][2].pos[1][0] = 1;
  p[5][2].pos[2][0] = 1;
  p[5][2].pos[3][0] = 1;

  p[5][3].pos[0][0] = 1;
  p[5][3].pos[0][1] = 1;
  p[5][3].pos[0][2] = 1;
  p[5][3].pos[0][3] = 1;

  //第七种图形的 4 种变形
  p[6][0].pos[0][0] = 1;
  p[6][0].pos[0][1] = 1;
  p[6][0].pos[1][0] = 1;
  p[6][0].pos[1][1] = 1;

  p[6][1].pos[0][0] = 1;
  p[6][1].pos[0][1] = 1;
  p[6][1].pos[1][0] = 1;
  p[6][1].pos[1][1] = 1;

  p[6][2].pos[0][0] = 1;
  p[6][2].pos[0][1] = 1;
  p[6][2].pos[1][0] = 1;
  p[6][2].pos[1][1] = 1;

  p[6][3].pos[0][0] = 1;
  p[6][3].pos[0][1] = 1;
  p[6][3].pos[1][0] = 1;
  p[6][3].pos[1][1] = 1;

}

void show_rock(struct tetris (*p)[4],int s,int r,int x,int y,int flag){
  int i,j;
  if(flag == 1){
    for(i = 0; i < 4; i++)
      for(j = 0; j < 4; j++){
        if(p[s][r].pos[i][j] == 1){
          printf("\033[%d;%dH",x+i,y+2*j);
          printf("[]");
          fflush(NULL);
        }
      }
  }
  if(flag == 0){
    for(i = 0; i < 4; i++)
      for(j = 0; j < 4; j++){
        if(p[s][r].pos[i][j] == 1){
          printf("\033[%d;%dH",x+i,y+2*j);
          printf("  ");
          fflush(NULL);
        }
      }
  }
}

int main(){
  int s,r;
  struct tetris t[7][4];
  struct termios term,save_term;
  
  //关闭回显
  if(tcgetattr(STDIN_FILENO,&term) < 0){
    perror("tcgetattr()");
    exit(1);
  }
  save_term = term;
  
  term.c_lflag &= ~(ECHO|ICANON); //关闭回显和规范模式
  term.c_cc[VTIME] = 0;
  term.c_cc[VMIN] = 1;

  if(tcsetattr(STDIN_FILENO,TCSANOW,&term) < 0){
    perror("tcsetattr()");
    exit(1);
  }
    //初始化 rock
  init_rock(t);
  print_square();

  s = rand() % 7;
  r = rand() % 4;
  while(1){
    show_rock(t,s,r,x,y,1);
    sleep(1);
    show_rock(t,s,r,x,y,0);
    x++;
  }
 
  //恢复终端标志  
  if(tcsetattr(STDIN_FILENO,TCSANOW,&save_term) < 0){
    perror("tcsetattr()");
    exit(1);
  }   

  exit(0);
}
