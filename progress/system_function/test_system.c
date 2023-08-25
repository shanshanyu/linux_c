#include <stdio.h>
#include <stdlib.h>

//对system的各种报错进行测试
//system失败的情况：fork失败，waitpid返回非EINTR的失败
//exec失败，system是成功的
//system的返回值：
//正常结束，system返回0
//exec命令不存在，system返回127
//


static void pr_exit(int status){
  if(WIFEXITED(status))
    printf("normal execute status = %d\n",WEXITSTATUS(status));
}


int main(){
  int status;
  status = system("date");
  pr_exit(status);

  status = system("nosuchcmd");
  pr_exit(status);

  status = system("who; exit 44");
  pr_exit(status);
  exit(0);

}
