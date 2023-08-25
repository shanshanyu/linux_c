#include "anytimer.h"
#include <stdio.h>
#include <stdlib.h>

void print_hello(void){
  printf("print 1\n");

}


int main(){
  timer *t = create_timer(print_hello,5);
  while(1)
    pause();
 
   exit(0);
}
