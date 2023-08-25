#include "mytbf.h"

//线程每秒钟对令牌桶的 token+cps
static void *thr_func(void *arg){
  mytbf_t *tbf = arg;
  int err;
  struct timespec tv;
  tv.tv_sec = 1;
  tv.tv_nsec = 0;
  while(1){
	  if(nanosleep(&tv,NULL) == -1){
		  perror("nanosleep()");
		  exit(1);
	  }

	  //超时时间到了，拿锁加时间
	  err = pthread_mutex_lock(&tbf->b_lock);
	  if(err != 0){
		  fprintf(stderr,"pthread_mutex_lock():%s\n",strerror(err));
		  exit(1);
	  }
	  tbf->token += tbf->cps;
	  if(tbf->token >= tbf->burst)
		  tbf->token = tbf->burst;
	  err = pthread_mutex_unlock(&tbf->b_lock);
	  if(err != 0){
		  fprintf(stderr,"pthread_mutex_unlock():%s\n",strerror(err));
		  exit(1);
	  }

  }
}

//初始化一个令牌桶
mytbf_t *mytbf_init(int cps,int burst){
  //初始化一块空间存放令牌桶
  mytbf_t *tbf = malloc(sizeof(mytbf_t));
  if(tbf == NULL)
    return NULL;
  
  int i,err;

  //令牌桶初始化
  tbf->token = 0;
  tbf->cps = cps;
  tbf->burst = burst;
  err = pthread_mutex_init(&tbf->b_lock,NULL);
  if(err != 0){
    fprintf(stderr,"pthread_mutex_init():%s\n",strerror(err));
    return NULL;
  }

  //找个位置存放令牌桶
  for(i = 0; i < MYTBF_MAX; i++){
    if(mytbf_arr[i] == NULL){
     mytbf_arr[i] = tbf;
     //把当前的位置记录到令牌桶中，方便后续释放
     tbf->pos = i;
     break;
    }
  }

  if(i == MYTBF_MAX)
    return NULL;

  //创建一个线程每秒钟对 token+cps
  err = pthread_create(&tbf->tid,NULL,thr_func,tbf);
  if(err != 0){
    fprintf(stderr,"pthread_create():%s\n",strerror(err));
    return NULL;
  }
  return tbf;
}

//取token，返回取到的token
int mytbf_fetchtoken(mytbf_t *tbf,int size){
  if(size <= 0)
    return -1;

  int err; 
  err = pthread_mutex_lock(&tbf->b_lock);
  if(err != 0){
    fprintf(stderr,"pthread_mutex_lock():%s\n",strerror(err));
    return -1;
  }
  //error here ???
  //如果没有可用的 token 要怎么处理？
  while(tbf->token <= 0){
    err = pthread_mutex_unlock(&tbf->b_lock);
    if(err != 0){
      fprintf(stderr,"pthread_mutex_unlock():%s\n",strerror(err));
      return -1;
    }
    sched_yield();
    err = pthread_mutex_lock(&tbf->b_lock);
    if(err != 0){
      fprintf(stderr,"pthread_mutex_lock():%s\n",strerror(err));
      return -1;
    }
  }
  int num = tbf->token < size ? tbf->token : size;
  tbf->token -= num;
  err = pthread_mutex_unlock(&tbf->b_lock);
  if(err != 0){
    fprintf(stderr,"pthread_mutex_unlock():%s\n",strerror(err));
    return -1;
  }
  
  return num;
}

//还token
int mytbf_returntoken(mytbf_t *tbf,int size){
  int err;
  err = pthread_mutex_lock(&tbf->b_lock);
  if(err != 0){
    fprintf(stderr,"pthread_mutex_lock():%s\n",strerror(err));
    return -1;
  }
  tbf->token += size;
  if(tbf->token > tbf->burst)
    tbf->token = tbf->burst;

  err = pthread_mutex_unlock(&tbf->b_lock);
  if(err != 0){
    fprintf(stderr,"pthread_mutex_unck():%s\n",strerror(err));
    return -1;
  }
  return size;
}

//销毁一个令牌桶
int mytbf_destroy(mytbf_t *tbf){
  int err;
  err = pthread_mutex_destroy(&tbf->b_lock);
  if(err != 0){
    fprintf(stderr,"pthread_mutex_destory():%s\n",strerror(err));
    return -1;
  }
  mytbf_arr[tbf->pos] = NULL;
  free(tbf);
  return 0;
}
