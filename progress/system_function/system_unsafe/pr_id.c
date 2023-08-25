#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  uid_t uid,euid,suid;

  getresuid(&uid,&euid,&suid);
  printf("uid = %d,euid = %d,suid = %d\n",uid,euid,suid);
  exit(0);
}
