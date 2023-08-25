#include "proto.h"

enum FSM_STATE{
  STATE_S_PATH,
  STATE_R_DATA,
  STATE_Ex,
  STATE_T
};

//客户端的状态机结构
typedef struct fsm_st{
  int msgid;
  enum FSM_STATE state;
  msg_path_t path;
  union s2c_st u_data;
  int fd;
}fsm_st;
