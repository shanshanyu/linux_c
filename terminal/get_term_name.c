#include <stdio.h>
#include <stdlib.h>

int main(){
  printf("terminal_name = %s\n",ctermid(NULL));

  exit(0);
}
