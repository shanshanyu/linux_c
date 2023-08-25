#include <stdio.h>
#include <stdlib.h>

int main(){
  enum week{Mon = 1,Tues,Wed,Thurs = 10,Fri,Sat,Sun};
  printf("%d %d %d %d %d %d %d\n",Mon,Tues,Wed,Thurs,Fri,Sat,Sun);
  enum week e;
  e = Wed;
  printf("e = %d\n",e);

  printf("enum size = %ld\n",sizeof(e));

  exit(0);
}
