#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define _GNU_SOURCE
//获取实际用户id，有效用户id，保存的设置用户id

int main(){

  uid_t uid,euid,suid;
  if(getresuid(&uid,&euid,&suid) == -1){
    perror("getresuid()");
    exit(1);
  }
  printf("uid= %d\n",uid);
  printf("euid= %d\n",euid);
  printf("suid= %d\n",suid);

  if(setuid(uid) != 0){
    fprintf(stderr,"setuid() faild\n");
    exit(1);
  }
   if(getresuid(&uid,&euid,&suid) == -1){
    perror("getresuid()");
    exit(1);
  }
  printf("uid= %d\n",uid);
  printf("euid= %d\n",euid);
  printf("suid= %d\n",suid);
  
   if(setuid(suid) != 0){
    fprintf(stderr,"setuid() faild\n");
    exit(1);
  }
   if(getresuid(&uid,&euid,&suid) == -1){
    perror("getresuid()");
    exit(1);
  }
  printf("uid= %d\n",uid);
  printf("euid= %d\n",euid);
  printf("suid= %d\n",suid);

  exit(0);
}
