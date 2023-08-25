#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

//延迟补偿算法
#define MAX_TIME 128  //最大超时时间 128s

int main(){
  int i;
  for(i = 1; i <= MAX_TIME; i <<= 1){
    //connect,如果成功就退出
    //最后一次 i=MAX_TIME 的时候如果connect 失败了，就不应该再执行 sleep
    if(4 < 3){
      printf("over\n");
      exit(1);
    }
    if(i < MAX_TIME){  //if(i <= MAX_TIME/2)
      sleep(i);
      printf("sleep %d s\n",i);

    }
  }

  exit(0);
}
