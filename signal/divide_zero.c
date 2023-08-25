#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void fpe_handler(int signo){
  printf("receive signal:%d\n",signo);
}

int main(){
  signal(SIGFPE,fpe_handler);
  printf("%d\n",3/0);
  exit(0);
}
