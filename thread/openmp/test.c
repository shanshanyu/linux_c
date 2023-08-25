#include <stdio.h>
#include <stdlib.h>

//openmp 标准下的多线程

int main(){

#pragma omp parallel
{
  puts("hello");
  puts("world");
}
  exit(0);
}
