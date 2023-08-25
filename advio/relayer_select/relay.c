#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "my_fsm.h"
#include "relay.h"
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#define JOB_MAX_SIZE 10000

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

static pthread_once_t once_init = PTHREAD_ONCE_INIT;

static pthread_t tid;

static job_st *job_arr[JOB_MAX_SIZE];

//线程推所有的状态机
static void *thr_func(void *arg){
  //布置监视任务
  //监视
  //查看监视结果 
  int i;
  int err;
  fd_set readfds,writefds;
  while(1){
    err = pthread_mutex_lock(&mut);
    if(err < 0){
      fprintf(stderr,"lock job_arr failed:%s\n",strerror(err));
      exit(1);
    }
    //布置监视任务  
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    for(i = 0; i < JOB_MAX_SIZE; i++){
      if(job_arr[i]){
        if(job_arr[i]->state == STATE_RUNNING){
          //两个fd的读写都感兴趣,要根据状态机的初始状态来初始化fd_set
          //例如：如果不根据状态机的状态来设置fd_set,每个fd都设置感兴趣读写事件，状态机的初始状态是读状态，如果状态机fd上有写事件发生
          //那么select会正常返回，然后去跑状态机，状态机的状态又是读，读不到数据，状态机结束，又继续select循环a
          if(job_arr[i]->fsm_src.state == STATE_R)
            FD_SET(job_arr[i]->fsm_src.fd_src,&readfds);
          if(job_arr[i]->fsm_src.state == STATE_W)
            FD_SET(job_arr[i]->fsm_src.fd_dst,&writefds);
          if(job_arr[i]->fsm_dst.state == STATE_R)
            FD_SET(job_arr[i]->fsm_dst.fd_src,&readfds);
          if(job_arr[i]->fsm_dst.state == STATE_W)
            FD_SET(job_arr[i]->fsm_dst.fd_dst,&writefds);
        }
        //如果状态机的状态是 STATE_Ex，无条件推动
        if(job_arr[i]->fsm_src.state == STATE_Ex)
          fsm_driver(&job_arr[i]->fsm_src);
        if(job_arr[i]->fsm_dst.state == STATE_Ex)
          fsm_driver(&job_arr[i]->fsm_dst);
      }
    }
 
    err = pthread_mutex_unlock(&mut);
    if(err < 0){
      fprintf(stderr,"unlock job_arr failed:%s\n",strerror(err));
      exit(1);
    }   
    
    //监视
    err = select(FD_SETSIZE,&readfds,&writefds,NULL,NULL);
    printf("err = %d\n",err);
    if(err < 0){
      perror("select()");
      exit(1);
    }else if(err == 0){
      //这种情况不存在，先占个位
      ;
    }else{
      //查看监视结果
      err = pthread_mutex_lock(&mut);
      if(err < 0){
        fprintf(stderr,"lock job_arr failed:%s\n",strerror(err));
        exit(1);
      }
      for(i = 0; i < JOB_MAX_SIZE; i++){
        if(job_arr[i]){
          if(FD_ISSET(job_arr[i]->fsm_src.fd_src,&readfds) || FD_ISSET(job_arr[i]->fsm_src.fd_dst,&writefds))
            fsm_driver(&job_arr[i]->fsm_src);
          if(FD_ISSET(job_arr[i]->fsm_dst.fd_src,&readfds) || FD_ISSET(job_arr[i]->fsm_dst.fd_dst,&writefds))
            fsm_driver(&job_arr[i]->fsm_dst);

          if(job_arr[i]->fsm_src.state == STATE_T && job_arr[i]->fsm_dst.state == STATE_T)
            job_arr[i]->state = STATE_OVER;
        }
      }
 
      err = pthread_mutex_unlock(&mut);
      if(err < 0){
        fprintf(stderr,"unlock job_arr failed:%s\n",strerror(err));
        exit(1);
      }
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

