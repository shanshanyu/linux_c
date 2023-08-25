#include "mytbf.h"


//mytbf_t 指针数组，用来存放令牌桶的指针
static mytbf_t *mytbf_arr[MYTBF_MAX];
//全局令牌锁,用来锁令牌桶数组（对令牌桶的创建，销毁等过程会用到）
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

//函数第一次执行的时候执行
static int inited = 1;

//线程id，用来执行 +token 逻辑的线程
static pthread_t tid;

//打印错误信息
static void err_print(const char *s,const int err){
   fprintf(stderr,"%s err:%s\n",s,strerror(err));
   exit(1);
}

//线程每秒钟对令牌桶的 token+cps
static void *thr_func(void *arg){
  int i,err;
  struct timespec tv;
  tv.tv_sec = 1;
  tv.tv_nsec = 0;
  while(1){
    if(nanosleep(&tv,NULL) == -1){
      perror("nanosleep()");
      exit(1);
    }
   
    //锁住整个令牌桶数组，因为需要查看令牌桶是否为空
    err = pthread_mutex_lock(&mut);
    if(err != 0)
      err_print("pthread_mutex_lock()",err);

    //遍历令牌桶数组，找到非空令牌桶，对token+cps
    for(i = 0; i < MYTBF_MAX; i++){
        if(mytbf_arr[i]){
        //锁住令牌桶
        err = pthread_mutex_lock(&mytbf_arr[i]->b_lock); 
        if(err != 0)
          err_print("pthread_mutex_lock()",err);
       
        mytbf_arr[i]->token += mytbf_arr[i]->cps;
        if(mytbf_arr[i]->token > mytbf_arr[i]->burst)
          mytbf_arr[i]->token = mytbf_arr[i]->burst;
        err = pthread_mutex_unlock(&mytbf_arr[i]->b_lock);
        if(err != 0)
          err_print("pthread_mutex_unlock()",err);
      }

    }
    err = pthread_mutex_unlock(&mut);
    if(err != 0)
      err_print("pthread_mutex_unlock()",err);
  }
}

//模块卸载
//需要对线程进行回收,怎么回收？线程 id 保存在 module_load 函数中
//解：把线程 id 定义成全局变量
//unload只能调用一次
static void module_unload(void){
  int i,err;
  //取消线程
  err = pthread_cancel(tid);
  if(err)
    err_print("pthread_cancel()",err);
  
  printf("tid = %d\n",tid);
  //回收线程资源
  err = pthread_join(tid,NULL);
  if(err)
    err_print("pthread_join()",err);
  printf("join success\n");
  for(i = 0; i < MYTBF_MAX; i++)
    if(mytbf_arr[i])
      mytbf_destroy(mytbf_arr[i]);
}
//模块加载
static void module_load(void){
  int err;
  if(inited){
    //如果多个线程同时调用 mytbf_init，inited 会变成一个竞争条件
    inited = 0;
    err = pthread_create(&tid,NULL,thr_func,NULL);
    if(err != 0)
      err_print("pthread_create()",err);
  }
  atexit(module_unload);
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
  if(err != 0)
    err_print("pthread_mutex_init",err);

  //找个位置存放令牌桶,令牌桶数组
  //加锁
  err = pthread_mutex_lock(&mut);
  if(err != 0)
    err_print("pthread_mutex_lock()",err);
  for(i = 0; i < MYTBF_MAX; i++){
    if(mytbf_arr[i] == NULL){
     mytbf_arr[i] = tbf;
     //把当前的位置记录到令牌桶中，方便后续释放
     tbf->pos = i;
     break;
    }
  }
  //令牌桶满了打印错误信息
  if(i == MYTBF_MAX){
    //解锁
    err = pthread_mutex_unlock(&mut);
    if(err != 0)
      err_print("pthread_mutex_unlock()",err);
    fprintf(stderr,"token bucket array is full,please init again\n");
    return NULL;
  }
  //解锁
  err = pthread_mutex_unlock(&mut);
  if(err != 0)
    err_print("pthread_mutex_unlock()",err);
  //模块加载
  module_load();
  return tbf;
}

//取token，返回取到的token
int mytbf_fetchtoken(mytbf_t *tbf,int size){
  if(size <= 0)
    return -1;

  int err; 
  err = pthread_mutex_lock(&tbf->b_lock);
  if(err != 0)
    err_print("pthread_mutex_lock()",err);
  //error here ???
  //如果没有可用的 token 要怎么处理？
  //循环等待，加过锁后不能跳转，如果报错，程序直接退出
  while(tbf->token <= 0){
    err = pthread_mutex_unlock(&tbf->b_lock);
    if(err != 0)
      err_print("pthread_mutex_unlock()",err);
    sched_yield();
    err = pthread_mutex_lock(&tbf->b_lock);
    if(err != 0)
      err_print("pthread_mutex_lock()",err);
  }

  int num = tbf->token < size ? tbf->token : size;
  tbf->token -= num;
  err = pthread_mutex_unlock(&tbf->b_lock);
  if(err != 0)
    err_print("pthread_mutex_unlock()",err);
  
  return num;
}

//还token
int mytbf_returntoken(mytbf_t *tbf,int size){
  int err;

  err = pthread_mutex_lock(&tbf->b_lock);
  if(err != 0)
    err_print("pthread_mutex_lock()",err);

  tbf->token += size;
  if(tbf->token > tbf->burst)
    tbf->token = tbf->burst;

  err = pthread_mutex_unlock(&tbf->b_lock);
  if(err != 0)
    err_print("pthread_mutex_unlock()",err);
  return size;
}

//销毁一个令牌桶
int mytbf_destroy(mytbf_t *tbf){
  int err;
  //令牌桶数组加锁
  err = pthread_mutex_lock(&mut);
  if(err != 0)
    err_print("pthread_mutex_lock()",err);

  mytbf_arr[tbf->pos] = NULL;
  //令牌桶数组解锁
  err = pthread_mutex_unlock(&mut);
  if(err != 0)
    err_print("pthread_mutex_unlock()",err);

  //令牌桶释放锁资源
  err = pthread_mutex_destroy(&tbf->b_lock);
  if(err != 0)
    err_print("pthread_mutex_destroy()",err);
  free(tbf);
  return 0;
}
