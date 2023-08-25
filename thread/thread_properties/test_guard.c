#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


int main(){
  pthread_attr_t attr;
  int err;
  size_t guard_size;
  //初始化attr结构
  err = pthread_attr_init(&attr);
  if(err != 0){
    fprintf(stderr,"pthread_attr_init() failed:%s\n",strerror(err));
    exit(1); 
  }

  err = pthread_attr_getguardsize(&attr,&guard_size);
  if(err != 0){
    fprintf(stderr,"pthread_attr_getguardsize() failed:%s\n",strerror(err));
    exit(1);
  }
  //guard_size 大小限制为 4k
  printf("guardsize = %d\n",guard_size);

  //反初始化attr结构  
  pthread_attr_destroy(&attr);

  exit(0);
}
