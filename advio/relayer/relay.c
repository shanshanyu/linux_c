#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "my_fsm.h"
#include "relay.h"
#include <pthread.h>
#define JOB_MAX_SIZE 10000

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

static pthread_once_t once_init = PTHREAD_ONCE_INIT;

static pthread_t tid;

static job_st *job_arr[JOB_MAX_SIZE];

//线程推所有的状态机
static void *thr_func(void *arg){
  int i;
  int err;
  while(1){
    err = pthread_mutex_lock(&mut);
    if(err < 0){
      fprintf(stderr,"lock job_arr failed:%s\n",strerror(err));
      exit(1);
    }

    for(i = 0; i < JOB_MAX_SIZE; i++){
      if(job_arr[i]){
        //这里不能加入2个状态机的T状态去做判断，不然执行不到下面改job的状态。
        if(job_arr[i]->state == STATE_RUNNING){
          fsm_driver(&job_arr[i]->fsm_src);
          fsm_driver(&job_arr[i]->fsm_dst);
          //job 如何推动到 STATE_OVER，在 2 个状态机的状态都变为 STATE_T的时候
          if(job_arr[i]->fsm_src.state == STATE_T && job_arr[i]->fsm_dst.state == STATE_T)
            job_arr[i]->state = STATE_OVER;
        }
      }
    }

    err = pthread_mutex_unlock(&mut);
    if(err < 0){
      fprintf(stderr,"unlock job_arr failed:%s\n",strerror(err));
      exit(1);
    }

  }
  return NULL;
}

static void module_unload(){
  int i,err;
  //回收线程资源
  err = pthread_cancel(tid);
  if(err < 0){
    fprintf(stderr,"cancel thread failed:%s\n",strerror(err));
    exit(1);
  }

  err = pthread_join(tid,NULL);
  if(err < 0){
    fprintf(stderr,"join thread failed:%s\n",strerror(err));
    exit(1);
  }

  //释放 job_arr
  err = pthread_mutex_lock(&mut);
  if(err < 0){
    fprintf(stderr,"lock job_arr failed:%s\n",strerror(err));
    exit(1);
  }

  for(i = 0; i < JOB_MAX_SIZE; i++){
    if(job_arr[i])
      free(job_arr[i]);
  }

  err = pthread_mutex_unlock(&mut);
  if(err < 0){
    fprintf(stderr,"unlock job_arr failed:%s\n",strerror(err));
    exit(1);
  }
  printf("module_unload success\n");
}


static void module_load(){
  pthread_create(&tid,NULL,thr_func,NULL);
  atexit(module_unload);
}

int add_job(int fd1,int fd2){
  /*
 *  成功返回job id
 * -EINVAL 出错，参数非法
 * -ENOSPC  出错，job数组满了
 * -ENOMEM  出错，malloc失败了
 */

  //保存fd1和fd2之前的文件状态标志
  int i;
  int err;
  job_st *job = malloc(sizeof(job_st));
  if(job == NULL){
    return -ENOMEM;
  }
  
  if(fd1 < 0 || fd2 < 0){
    return -EINVAL;
  }

  job->fdl_save = set_fl(fd1,O_NONBLOCK);
  job->fdr_save = set_fl(fd2,O_NONBLOCK);

  job->fsm_src.fd_src = fd1;
  job->fsm_src.fd_dst = fd2;
  job->fsm_src.state = STATE_R;
  job->fsm_dst.fd_src = fd2;
  job->fsm_dst.fd_dst = fd1;
  job->fsm_dst.state = STATE_R;
  job->state = STATE_RUNNING;
  //两个状态机初始化完成
 
  err = pthread_mutex_lock(&mut);
  if(err < 0){
    fprintf(stderr,"pthread_mutex_lock failed:%s\n",strerror(err));
    exit(1);
  }
    
  for(i = 0; i < JOB_MAX_SIZE; i++){
    if(!job_arr[i]){
      job_arr[i] = job;
      job->pos = i;
      break;
    }
  }
  err = pthread_mutex_unlock(&mut);
  if(err < 0){
    fprintf(stderr,"pthread_mutex_unlock failed:%s\n",strerror(err));
    exit(1);
  }
  
  if(i == JOB_MAX_SIZE)
    return -ENOMEM;
  //这里缺少对资源的释放

  
  err = pthread_once(&once_init,module_load); 
  if(err < 0){
    fprintf(stderr,"pthread_once:%s\n",strerror(err));
    exit(1);
  }

  return job->pos; 
}


int del_job(int job_id){
  int err;
  int i = job_id;
  if(job_id < 0)
    return -EINVAL;
  
  err = pthread_mutex_lock(&mut);
  if(err < 0){
    fprintf(stderr,"pthread_mutex_lock failed:%s\n",strerror(err));
    exit(1);
  }
  job_arr[i]->state = STATE_OVER;
  fcntl(job_arr[i]->fsm_src.fd_src,job_arr[i]->fdl_save);
  fcntl(job_arr[i]->fsm_src.fd_dst,job_arr[i]->fdr_save);
  
  free(job_arr[i]);
  job_arr[i] = NULL;

  err = pthread_mutex_unlock(&mut);
  if(err < 0){
    fprintf(stderr,"pthread_mutex_unlock failed:%s\n",strerror(err));
    exit(1);
  } 
  return 0;
}

