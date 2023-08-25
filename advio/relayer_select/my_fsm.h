#ifndef MY_FSM_H
#define MY_FSM_H
#define BUFSIZE 1024

enum FSM_STATE{
  STATE_R,
  STATE_W,
  STATE_Ex,
  STATE_T 
};

typedef struct fsm_st{
  int fd_src;
  int fd_dst;
  enum FSM_STATE state;
  int rd_ret;
  char buf[BUFSIZE];
  int pos; 
  char *err_str; 
}fsm_st;

/*void relay(int jid); */
//仅有一个接口

void fsm_driver(fsm_st *fsm);
int set_fl(int fd,int flags);
#endif
