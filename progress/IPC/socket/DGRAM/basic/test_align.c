#include <stdio.h>
#include <stdlib.h>
struct msg_st{
  int a;
  char b;
};
//}__attribute__((packed));


int main(){
  printf("msg_st size: %ld\n",sizeof(struct msg_st));

  exit(0);
}
