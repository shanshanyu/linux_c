#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(){
  mode_t cur_mask;

  cur_mask = umask(0);

  printf("current umask = %o",cur_mask);


  cur_mask = umask(0);

  printf("current umask = %o",cur_mask);
  exit(0);
}
