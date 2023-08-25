#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "mytbf.h"
//最大的令牌桶数量
#define MAX_BUCKET_SIZE 1024

static struct mytbf_t *mytbf_arr[MAX_BUCKET_SIZE];
//令牌桶数组的锁，操作令牌桶数组的时候要加锁
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static int inited = 1;

//创建一个单独的线程来定时加 token 
void *add_token(void *arg){
  int i;
  int err;
  struct timespec it;
  it.tv_sec = 1;
  it.tv_nsec = 0;

while(1){
   nanosleep(&it,NULL);
   //定时1s钟执行一次

  err = pthread_mutex_lock(&lock);
  if(err != 0){
    fprintf(stderr,"lock mytbf arr failed:%s\n",strerror(err));
    exit(1);
  }

  for(i = 0; i < MAX_BUCKET_SIZE; i++){
    if(mytbf_arr[i]){
      err = pthread_mutex_lock(&mytbf_arr[i]->mut);
      if(err != 0){
         fprintf(stderr,"lock mutex failed:%s\n",strerror(err));
         exit(1);
      }
      mytbf_arr[i]->token += mytbf_arr[i]->cps;
      printf("mytbf_arr[i]->token = %d\n",mytbf_arr[i]->token);
      //通知条件变化
      err = pthread_cond_broadcast(&mytbf_arr[i]->cond);
      if(err != 0){
        fprintf(stderr,"pthread_cond_broadcast failed:%s\n",strerror(err));
        exit(1);
      }
      err = pthread_mutex_unlock(&mytbf_arr[i]->mut);
      if(err != 0){
        fprintf(stderr,"unlock mutex failed:%s\n",strerror(err));
        exit(1);
      }
    }
  }

  err = pthread_mutex_unlock(&lock);
  if(err != 0){
    fprintf(stderr,"unlock mytbf arr failed:%s\n",strerror(err));
    exit(1);
  }
}
  return NULL;
}



//创建令牌桶
mytbf_t *create_mytbf(int token,int cps){
  int err;
  int i;
  pthread_t tid;
  //判断参数是否合法
  if(token <= 0 || cps <= 0){
     fprintf(stderr,"args invalid\n");
     return NULL;
  }
  //初始化令牌桶
  mytbf_t *t = malloc(sizeof(mytbf_t));
  if(t == NULL){
    fprintf(stderr,"malloc failed\n");
    return NULL;
  }

  t->cps = cps;
  t->token = token;
  err = pthread_mutex_init(&t->mut,NULL);
  if(err != 0){
    fprintf(stderr,"mutex init failed:%s\n",strerror(err));
    return NULL;
  }
  err = pthread_cond_init(&t->cond,NULL);
  if(err != 0){
    fprintf(stderr,"cond init failed:%s\n",strerror(err));
    return NULL;
  }
  //锁住令牌桶数组
  err = pthread_mutex_lock(&lock);
  if(err != 0){
    fprintf(stderr,"lock mytbf_arr failed:%s\n",strerror(err));
    return NULL;
  }

  for(i = 0; i < MAX_BUCKET_SIZE; i++){
    if(mytbf_arr[i] == NULL){
      mytbf_arr[i] = t;
      t->index = i; //记录令牌桶在数组中的位置
      break;
    }
  }
  if(i == MAX_BUCKET_SIZE){
    fprintf(stderr,"mytbf_arr is full,create token failed\n");
    return NULL;
  }
  
  err = pthread_mutex_unlock(&lock);
  if(err != 0){
    fprintf(stderr,"unlock failed:%s\n",strerror(err));
    return NULL;
  }
  //初始化
  if(inited == 1){
    err = pthread_create(&tid,NULL,add_token,NULL);
    if(err != 0){
       fprintf(stderr,"create thread failed:%s\n",strerror(err));
       return NULL;
     }
    inited = 0;
  }

  return t;

}


int destroy_mytbf(mytbf_t *t){
  if(t == NULL){
    fprintf(stderr,"args invalid\n");
    return -1;
  }
  int err;
  //锁住令牌桶数组
  err = pthread_mutex_lock(&lock);
  if(err != 0){
    fprintf(stderr,"lock mytbf_arr failed:%s\n",strerror(err));
    return -1;
  }

  mytbf_arr[t->index] = NULL;
  err = pthread_cond_destroy(&t->cond);
  if(err != 0){
    fprintf(stderr,"destroy cond failed:%s\n",strerror(err));
    return -1;
  }
  err = pthread_mutex_destroy(&t->mut);
  if(err != 0){
    fprintf(stderr,"destroy mutex failed:%s\n",strerror(err));
    return -1;
  }

  //释放内存
  free(t);

  err = pthread_mutex_unlock(&lock);
  if(err != 0){
    fprintf(stderr,"unlock failed:%s\n",strerror(err));
    return -1;
  }

  return 0;
}
////取令牌
int fetch_token(mytbf_t *t,int token){
  if(t == NULL || token <= 0){
    fprintf(stderr,"args invalid\n");
    return -1;
  }
  //取token，如果存的 token 不足怎么办？
  int err;
  err = pthread_mutex_lock(&t->mut);
  if(err != 0){
    fprintf(stderr,"lock mutex failed:%s\n",strerror(err));
    return -1;
  }

  while(t->token < token){
     printf("token = %d\n",t->token);
     err = pthread_cond_wait(&t->cond,&t->mut);
     if(err != 0){
       fprintf(stderr,"pthread_cond_wait failed:%s\n",strerror(err));
       return -1;
     }
  }
  t->token -= token;
  printf("second token = %d\n",t->token);
  err = pthread_mutex_unlock(&t->mut);
  if(err != 0){
     fprintf(stderr,"pthread_mutex_unlock failed:%s\n",strerror(err));
     return -1;
   }

  return token;
} 

//还 token
int return_token(mytbf_t *t,int token){
  if(t == NULL || token < 0){
    fprintf(stderr,"args invalid\n");
    return 0;
  }
  
  int err;
  err = pthread_mutex_lock(&t->mut);
  if(err != 0){
    fprintf(stderr,"lock mutex failed:%s\n",strerror(err));
    return -1;
  }

  t->token += token;

  err = pthread_cond_broadcast(&t->cond);
  if(err != 0){
    fprintf(stderr,"pthread_cond_broadcast failed:%s\n",strerror(err));
    return -1;
  }
  err = pthread_mutex_unlock(&t->mut);
  if(err != 0){
     fprintf(stderr,"pthread_mutex_unlock failed:%s\n",strerror(err));
     return -1;
   }
  return token;
}
