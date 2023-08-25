#ifndef MYTBF_H
#define MYTBF_H
typedef struct mytbf_t{
  int token; //令牌数目
  int cps; //流速
  int index; //记录令牌桶在令牌桶数组中的位置
  pthread_mutex_t mut; //令牌桶的锁
  pthread_cond_t cond; //添加条件变量，在需要非阻塞等的时候用
}mytbf_t;
//创建令牌桶
mytbf_t *create_mytbf(int token,int cps);
int destroy_mytbf(mytbf_t *t);
//取令牌
int fetch_token(mytbf_t *t,int token);
int return_token(mytbf_t *t,int token);

#endif
