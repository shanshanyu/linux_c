#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define BUFSIZE 1024

int main(){
  char buf[BUFSIZE];
  struct termios term,term_save;
  if(tcgetattr(STDIN_FILENO,&term)  < 0){
    perror("tcgetattr()");
    exit(1);
  }

  term_save = term;
  printf("input your pass: ");

  term.c_lflag  &= ECHOK; //关闭回显
  if(tcsetattr(STDIN_FILENO,TCSANOW,&term) < 0){
    perror("tcsetattr()");
    exit(1);
  }

  scanf("%s",buf);
  
  if(tcsetattr(STDIN_FILENO,TCSANOW,&term_save) < 0){
    perror("tcsetattr()");
    exit(1);
  }
  printf("passwd: %s\n",buf);
  exit(0);
}
