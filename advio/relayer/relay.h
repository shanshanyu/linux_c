#ifndef RELAY_H
#define RELAY_H

#define JOB_MAX_SIZE 10000

enum JOB_STATE{
  STATE_RUNNING,
  STATE_OVER

};

//job结构
typedef struct job_st{
//保存两个状态机
  fsm_st fsm_src;
  fsm_st fsm_dst;
  enum JOB_STATE state;
  int pos; //指示job在job数组中的位置
  int fdl_save;
  int fdr_save;
}job_st;


int add_job(int fd1,int fd2);
int del_job(int job_id);
#endif
