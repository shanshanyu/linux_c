#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int signo){
  printf("receive sigint\n");
}

int main(){
  signal(SIGINT,sig_handler);
  while(1)
    pause();
  exit(0);
}
