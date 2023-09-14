#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(){
  time_t t;
  time(&t);
  printf("%d\n",t);

  exit(0);
}
