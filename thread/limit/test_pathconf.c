#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  printf("指定目录中允许的普通文件最大长度所需的最小位数：%d\n",pathconf("/tmp",_PC_FILESIZEBITS));
  printf("文件链接计数的最大值:%d\n",pathconf("/tmp/out",_PC_LINK_MAX));
  printf("终端规范输入队列的最大字节数:%d\n",fpathconf(STDIN_FILENO,_PC_MAX_CANON));
  printf("终端输入队列可用空间的字节数:%d\n",fpathconf(STDIN_FILENO,_PC_MAX_INPUT));
  printf("文件名的最大字节数:%d\n",pathconf("/tmp/out",_PC_NAME_MAX));
  printf("相对路径的最大字节数:%d\n",pathconf("/tmp/out",_PC_PATH_MAX));
  printf("能原子的写到管道的最大字节数:%d\n",pathconf("/tmp",_PC_PIPE_BUF));
  printf("符号链接的字节数:%d\n",pathconf("/tmp",_PC_SYMLINK_MAX));


  exit(0);
}
