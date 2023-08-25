//条件编译,解决重复包含头文件的问题
#ifndef MYTBF_H__
#define MYTBF_H__
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
//alarm pause headfile
#include <unistd.h>


typedef struct mytbf_st{
  int token; //当前拥有的令牌
  int cps; //速率
  int burst; //上限
  int pos; //记录当前令牌桶在令牌桶数组中的位置
}mytbf_t;

//初始化一个令牌桶
mytbf_t *mytbf_init(int cps,int burst);

//取token，返回取到的token
int mytbf_fetchtoken(mytbf_t *,int);

//还token
int mytbf_returntoken(mytbf_t *,int);

//销毁一个令牌桶
int mytbf_destroy(mytbf_t *);

#endif


