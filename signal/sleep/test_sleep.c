#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//调用sleep 后，就算没有达到闹钟超时时间，如果收到一个信号，那么也会导致sleep结束
static void usr1_handler(int sig){
  printf("recevice %d signal\n",sig);
}


int main(){
  signal(SIGUSR1,usr1_handler);
  printf("sleep remain %d\n",sleep(40));

  return 0;
}
