#include <stdio.h>
#include <stdlib.h>

#define MIN 30000000
#define MAX 30000200

int get_prime(int x){
  int j;
  int flag = 0;
  for(j = 2; j < x; j++){
    if(x % j == 0){
      break;
    }
  }
  if(x == j)
    flag = 1;

  if(flag == 1)
    printf("%d is primer\n",x);
  return 0;
}


int main(){
  int i; 
  for(i = MIN; i < MAX; i++)
    get_prime(i);

  exit(0);
}
