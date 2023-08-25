#include "proto.h"
#define BUFSIZE 1024

enum FSM_STATE{
  STATE_R_PATH,
  STATE_S_DATA
};

typedef struct fsm_st{
  int msgid;
  enum FSM_STATE state;
  msg_path_t path; //path 包
  union s2c_st u_data;  //data 包和 eot 包
  int open_flag;
  int fd;
}fsm_st;
