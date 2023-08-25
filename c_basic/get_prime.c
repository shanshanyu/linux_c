#include <stdio.h>
#include <stdlib.h>

#define MIN 30000000
#define MAX 30000200

int get_prime(int x){
  int j;
  for(j = 2; j < x; j++){ //没有必要遍历到 x-1,遍历到 x/2 -1 就行
    if(x % j == 0)
      return 1;
  }
  if(x == j)
    return 0;
}


int main(){
  int i; 
  for(i = MIN; i < MAX; i++)
    if(!get_prime(i))
      printf("%d is primer\n",i);

  exit(0);
}
