#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//创建N个子进程来计算质数

#define LEFT 30000000
#define RIGHT 30000200


//判断一个数是不是质数
void get_prime(int n){
  int i;
  int flag = 1;
  if(n == 1) printf("%d\n",n);
  if(n == 2) return ;
  
  for(i = 2; i < n/2; i++){
    if(n % i == 0){
      flag = 0;
      break;
    }
  }
  if(flag == 1)
    printf("%d\n",n);

}


int main(){
  //get_prime(3);
  int i;
  pid_t pid;
  for(i = LEFT; i <= RIGHT; i++){
    pid = fork();
    if(pid < 0){
      perror("fork()");
    }else if(pid == 0){
      get_prime(i);
      exit(0);  //少了这条语句可不得了，会死机的
    }
  }
  sleep(10);
  exit(0);
}
