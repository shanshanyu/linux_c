#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 *5s 自加，打印，不停循环
 *
 */

int main(){
  int i = 0;
  time_t t;
  t = time(NULL);
  while(time(NULL) < t+5)
    i++;

  printf("%d\n",i); 

  exit(0);
}
