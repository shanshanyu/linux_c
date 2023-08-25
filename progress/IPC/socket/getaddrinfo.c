#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>


void print_flags(struct addrinfo *aip){
  printf(" flags ");
  if(aip->ai_flags & AI_ADDRCONFIG)
      printf("AI_ADDRCONFIG\n");
  else if(aip->ai_flags & AI_ALL)
      printf("AI_ALL\n");
  else if(aip->ai_flags & AI_CANONNAME)
      printf("AI_CANNONNAME\n");
  else if(aip->ai_flags & AI_PASSIVE)
      printf("AI_PASSIVE\n");
  else
      printf("unknow\n");
}

void print_protocol(struct addrinfo *aip){
  printf(" procotol ");
  switch(aip->ai_protocol){
    case IPPROTO_IP:
      printf("IPPROTO_IP\n");
      break;
    case IPPROTO_IPV6:
      printf("IPPROTO_IPV6\n");
      break;
    case IPPROTO_ICMP:
      printf("IPPROTO_ICMP\n");
      break;
    case IPPROTO_RAW:
      printf("IPPROTO_RAW\n");
      break;
    case IPPROTO_TCP:
      printf("IPPROTO_TCP\n");
      break;
    case IPPROTO_UDP:
      printf("IPPTORO_UDP\n");
      break;
    default:
      printf("unknown\n");
  }

}

int main(int argc,const char *argv[]){
  struct addrinfo *ailist,*aip;
  struct addrinfo hint;
  int err;

  struct sockaddr_in *sinp;
  char buf[INET_ADDRSTRLEN];
  const char *addr;

  if(argc != 2){
    fprintf(stderr,"Usage: %s name services\n",argv[0]);
    exit(1);
  }

  //初始化 hint
  hint.ai_family = 0;
  hint.ai_flags = AI_PASSIVE;
  hint.ai_protocol = 0;
  hint.ai_socktype = 0;
  hint.ai_addrlen = 0;
  hint.ai_addr = NULL;
  hint.ai_canonname = NULL;
  hint.ai_next = NULL;

  
  err = getaddrinfo(NULL,argv[1],&hint,&ailist);
  if(err != 0){
    fprintf(stderr,"getaddrinfo failed:%s\n",gai_strerror(err));
    exit(1);
  }

  for(aip = ailist; aip != NULL; aip = aip->ai_next){
    print_flags(aip);
    print_protocol(aip);
  
    if(aip->ai_family == AF_INET){
      sinp = (struct sockaddr_in *)aip->ai_addr;
      addr = inet_ntop(AF_INET,&sinp->sin_addr,buf,INET_ADDRSTRLEN);
      printf(" address %s\n",addr?addr:"unknow");
      printf(" port %d\n",sinp->sin_port?sinp->sin_port:0);
    }
  }
    

  exit(0);
}
