#include <stdio.h>
#include <stdlib.h>

//质数：大于 1 的自然数中只能被 1 个自身整除的数

#define MIN 30000000
#define MAX 30000200

void get_prime(int x){
  int flag = 1;
  int i;
  //质数计算逻辑，只能被 1 和自身整除，不用包含 x/2，因为如果能被 x/2 整除，也能被 2 整除
  for(i = 2; i < x/2; i++){
    if(x % i == 0){
      flag = 0;
      break;
    }
  }

  if(flag == 1) printf("%d is primer\n",x);
}


int main(){
  int i;
  for(i = MIN; i <= MAX; i++)
    get_prime(i);

  exit(0);
}
