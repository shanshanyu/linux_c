#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//查看 IOV_MAX 的值

int main(){
  printf("IOV_MAX = %ld\n",sysconf(_SC_IOV_MAX));

  return 0;
}
