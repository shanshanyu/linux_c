#include <stdio.h>
#include <stdlib.h>


int main(){
  char s[] = "abc";
  printf("sizeof(s) = %ld\n",sizeof(s));
  printf("s[1] = %c\n",s[1]);

  char *p = s;
  printf("sizeof(p) = %ld\n",sizeof(p));
  printf("p[1] = %c\n",p[1]);


  exit(0);
}
