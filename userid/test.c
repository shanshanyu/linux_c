#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  uid_t uid;
  uid_t euid;
  uid_t suid;
  printf("pid = %d\n",getpid());
  printf("uid = %d\n",getuid());
  printf("euid = %d\n",geteuid());
  
  if(getresuid(&uid,&euid,&suid) != 0){
    fprintf(stderr,"getresuid() err\n");
    exit(1);
   }
  printf("uid=%d,euid=%d,suid=%d\n",uid,euid,suid);
 
  sys.
  return 0;


}
