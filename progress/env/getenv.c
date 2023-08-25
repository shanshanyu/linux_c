#include <stdio.h>
#include <stdlib.h>


int main(){
  printf("HOME ENV:%s\n",getenv("HOME"));
  printf("TZ  ENV:%s\n",getenv("TZ"));

  exit(0);
}
