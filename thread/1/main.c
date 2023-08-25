#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main(){
  printf("hello\n");
  pthread_exit(0);
}
