#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//查看系统存储页大小  PAGE_SIZE  PAGESIZE
//4k

int main(){
  printf("pagesize = %ld\n",sysconf(_SC_PAGESIZE));
  printf("page_size = %ld\n",sysconf(_SC_PAGE_SIZE));

  exit(0);
}
