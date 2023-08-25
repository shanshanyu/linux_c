#include <stdio.h>
#include <stdlib.h>

/*
 *计算 30000000 30000200 之前的质数，一共有 18 个
 *
 *
 */
#define LEFT  30000000
#define RIGHT 30000200

static void get_prime(int num){
  int i;
  int flag;
  flag = 1;
  for(i = 2; i < num/2; i++)
    if(num % i == 0){
      flag = 0;
      break;
    }
  if(flag) printf("%d is primer\n",num);

}

int main(){
  int i;
  //当前程序是串行的,没有并发
  for(i = LEFT; i <= RIGHT; i++)
    get_prime(i);
  //返回启动例程
  return 0;
}

