#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc,const char *argv[]){
  int i;
  for(i = 0; argv[i] != NULL; i++)
    printf("argv[%d] = %s\n",i,argv[i]);
  for(i = 0; environ[i] != NULL; i++)
    printf("argv[%d] = %s\n",i,environ[i]);


  

  exit(0);
}
