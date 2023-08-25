#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

/*传递一个 ip 地址，判断 ip 地址是否合法，然后输出 ip 地址
 *
 *
 */

int main(int argc,const char *argv[]){
  if(argc != 3){
    fprintf(stderr,"Usage:%s i4/i6/number addr\n",argv[0]);
    exit(1);
  }

  int domain;
  unsigned char buf[sizeof(struct in6_addr)]; 
  int ret;
  char addr[INET6_ADDRSTRLEN];



  domain = strcmp(argv[1],"i4") == 0 ? AF_INET : strcmp(argv[1],"i6") == 0 ? AF_INET6 : atoi(argv[1]);

  ret = inet_pton(domain,argv[2],buf);
  if(ret <= 0){
    if(ret == 0){
      fprintf(stderr,"Address format is wrong\n");
    }else
      perror("inet_pton()");

    exit(1);
  }

  if(inet_ntop(domain,buf,addr,INET6_ADDRSTRLEN) < 0){
    perror("inet_pton()");
    exit(1);
  }
  printf("%s\n",addr);

  exit(0);
}
