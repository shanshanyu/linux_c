#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>


int main(int argc,const char *argv[]){
  if(argc != 2){
    fprintf(stderr,"Usage:%s protoname\n",argv[0]);
    exit(1);
  }

  struct protoent *ent;

  ent = getprotobyname(argv[1]);
  if(ent == NULL){
    fprintf(stderr,"getprotobyname() failed\n");
    exit(1);
  }

  puts(ent->p_name);
  printf("protocol number :%d\n",ent->p_proto);

  exit(0);
}
