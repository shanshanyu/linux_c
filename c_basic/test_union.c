#include <stdio.h>
#include <stdlib.h>

typedef struct msg_data_st{
  long mtype;
  char data[1024];
  int len;
}msg_data_t;

typedef struct msg_eot_st{
  long mtype;
}msg_eot_t;


union s2c_st{
  long mtype;
  msg_data_t u_data;
  msg_eot_t u_eot;
};

int main(){
  msg_data_t data;
  union s2c_st var;
  printf("union size = %ld\n",sizeof(var));
  printf("data size = %ld\n",sizeof(data));
  printf("long type size = %ld\n",sizeof(long));
  printf("int type size = %ld\n",sizeof(int));
  exit(0);
}
