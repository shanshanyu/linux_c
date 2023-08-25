#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main(){
  int err;
  pthread_mutex_t mut;
  pthread_mutex_init(&mut,NULL);
  printf("init\n");
  pthread_mutex_lock(&mut);
  printf("lock\n");
  err = pthread_mutex_unlock(&mut);
  if(err != 0)
    fprintf(stderr,"unlock failed:%s\n",strerror(err));
  printf("unlock\n");
  err = pthread_mutex_unlock(&mut);
    if(err != 0)
      fprintf(stderr,"unlock failed:%s\n",strerror(err));

  printf("unlock 2\n");
  exit(0);


}

/*
 *解锁两次不会阻塞
 *
 *
 */
