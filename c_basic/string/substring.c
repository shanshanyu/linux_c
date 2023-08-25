#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int cmp_str(const char *str1,const char *str2,int len){
  //比较两个字符串，相等返回0，不相等返回-1
  int i;
  int flag = 1;
  for(i = 0; i < len; i++){
    if(str1[i] != str2[i]){
      flag = 0;
      break;
    }
  }

  if(flag == 1)
    return 0;
  else
    return -1;
}

int main(int argc,const char *argv[]){
  int i;
  int src_len,dst_len;
  int flag = 0;
  int index;

  if(argc != 3){
    fprintf(stderr,"Usage:%s src_str  dst_str\n",argv[0]);
    exit(1);
  }

  src_len = strlen(argv[1]);
  dst_len = strlen(argv[2]);

  for(i = 0; i <= dst_len - src_len; i++){
    if(cmp_str(argv[1],argv[2]+i,src_len) == 0){
      flag = 1;
      index = i;
      break;
    }  
  }

  if(flag == 1)
    printf("find substr,index = %d\n",index);
  else
    printf("not find substr\n");  

  exit(0);
}
