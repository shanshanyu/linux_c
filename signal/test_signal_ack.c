#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void print_char(int status){
  write(STDOUT_FILENO,"!",1);
  sleep(1);

}


int main(){
  signal(SIGINT,print_char);
  while(1){
    write(STDOUT_FILENO,"*",1);
    sleep(1);
  } 

  exit(0);
}
