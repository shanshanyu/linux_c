#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>

int main(){
  struct hostent *host;
  host = gethostent();
  printf("h_name = %s,h_ddrtype=%d,h_lenght=%d\n",host->h_name,host->h_addrtype,host->h_length);
  printf("h_aliases = %s\n",*host->h_aliases);
  
  exit(0);
}
