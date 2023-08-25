#include "mytbf.h"

#define MYTBF_MAX 1000

typedef void (*sighandler_t)(int);
static sighandler_t sigalrm_save;

//定义令牌桶数组,最多可以有 MYTBF_MAX 个令牌桶
static mytbf_t *mytbf_arr[MYTBF_MAX];

//标志位，让 alarm 只执行一次
static int inited = 1;


void alrm_handler(int sig){
  //对每一个非空令牌桶的token+cps
  int i;
  for(i = 0; i < MYTBF_MAX; i++){
    if(mytbf_arr[i]){
      mytbf_arr[i]->token += mytbf_arr[i]->cps;
      if(mytbf_arr[i]->token > mytbf_arr[i]->burst)
        mytbf_arr[i]->token = mytbf_arr[i]->burst;
    }
  } 
  alarm(1);
}

int min(int a,int b){
  if(a < b) 
    return a;
  else 
    return b;
}

//模块初始化
static void module_load(){
    //保存之前的信号处理方式
    sigalrm_save = signal(SIGALRM,alrm_handler);
    if(sigalrm_save == SIG_ERR){
       perror("signal():");
       exit(1);
    }
     alarm(1);
}

static void module_unload(){
  int i;
  signal(SIGALRM,sigalrm_save);
  alarm(0);
  
  for(i = 0; i < MYTBF_MAX; i++){
    free(mytbf_arr[i]);
    mytbf_arr[i]  = NULL;
  }
}

//初始化一个令牌桶
mytbf_t *mytbf_init(int cps,int burst){
  mytbf_t *bucket = malloc(sizeof(mytbf_t));
  //在 main 中做判断，成功返回 bucket 指针，出错返回 NULL
  if(bucket == NULL) 
    return NULL;
  if(inited){
    module_load();
    //设置标志，这个标志在 mytbf_init 的什么位置设置都是 ok 的，会产生影响的只有在本次modudle调用没结束，而下次调用又开始，这种情况不会发生。下次调用 mytbf_int又开始了
    inited = 0;
    //注册钩子函数，在程序正常结束时调用
    atexit(module_unload);
  }
  int i;
  bucket->token = 0;
  bucket->cps = cps;
  bucket->burst = burst;

  //把令牌放到一个空位置
  for(i = 0; i < MYTBF_MAX; i++){
    if(!mytbf_arr[i]) {
      mytbf_arr[i] = bucket;
      //记录令牌桶的位置
      bucket->pos = i;
      break;
    }
  }
  //最后检查令牌桶数组是不是满的
  if(i == MYTBF_MAX)
    return NULL;
  else
    return bucket;
}

//取token，返回取到的token
int mytbf_fetchtoken(mytbf_t *ptr,int size){
  if(size <= 0)
    return -1;
  //如果当前没有可用的token，就等待
  while(ptr->token <= 0)
    pause();
  int n; 
  //判断 size 和 ptr->token 的大小
  n = min(ptr->token,size);
  /*if(ret == ptr->token){
    ptr->token = 0;
    return ptr->token;
  }
  else if(ret == size){
    ptr->token -= size;
    return ptr->token;
  }*/
  //ptr->token 减去当前可用的 token，剩余的 token
  ptr->token -= n;
  return n;
}

//还token
int mytbf_returntoken(mytbf_t *ptr,int size){
  if(size <= 0)
    return -1;

  ptr->token += size;
  if(ptr->token > ptr->burst)
    ptr->token = ptr->burst;
  return size;
}

//销毁一个令牌桶
int mytbf_destroy(mytbf_t *ptr){
  mytbf_arr[ptr->pos] = NULL;
  free(ptr);
  return 0; 
}
