#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE 1024

int main(){
  FILE *fp;
  char buf[BUFSIZE];
  fp = fopen("./test","r");

  fgets(buf,BUFSIZE,fp);
  fprintf(stdout,"%s",buf);

  printf("%ld\n",ftell(fp));
  fclose(fp);
  
  exit(0);
}
