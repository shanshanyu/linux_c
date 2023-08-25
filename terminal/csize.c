#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>


int main(){
  struct termios term;
  if(tcgetattr(STDIN_FILENO,&term)  == -1){
    perror("tcgetattr()");
    exit(1);
  }

  switch(term.c_cflag & CSIZE){
    case CS5:
      printf("5 bits/byte\n");
      break;
    case CS6:
      printf("6 bits/byte\n");
      break;
    case CS7:
      printf("7 bits/byte\n");
      break;
    case CS8:
      printf("8 bits/byte\n");
      break;
    default:
      printf("unknow flag\n");
  }

  exit(0);
}
