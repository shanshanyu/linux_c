#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include "op_pipe.h"
#include "my_pipe.h"

#define P_FD_SIZE 1024

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

struct pipe_st{
  //原始结构
  my_pipe_t *p;
  //管道操作权限
  int opmap;
};

struct pipe_st *p_fd_arr[P_FD_SIZE];

int open_pipe(int p_fd,int opmap){
  //查看管道是否被创建过
  pthread_mutex_lock(&mut);
  if(!p_fd_arr[p_fd]){
    struct pipe_st *me = malloc(sizeof(struct pipe_st));
    if(me == NULL){
       pthread_mutex_unlock(&mut);
       return -1;
    }

    my_pipe_t *p = create_pipe();
    if(p == NULL){
       pthread_mutex_unlock(&mut);
       return -1;
    }

    me->p = p;
    p_fd_arr[p_fd] = me;
  }
  p_fd_arr[p_fd]->opmap |= opmap;
  pthread_mutex_unlock(&mut);
  //注册权限
  if(register_client(p_fd_arr[p_fd]->p,opmap) != 0)
    return -1;
  printf("open success\n");
  return 0;
}

int close_pipe(int p_fd){
  struct pipe_st *me = p_fd_arr[p_fd];
  if(destroy_pipe(me->p)  != 0)
    return -1;
  
  free(p_fd_arr[p_fd]);
  p_fd_arr[p_fd] = NULL;

  return 0;
}


int read_pipe_new(int p_fd,void *buf,size_t count){
  struct pipe_st *me = p_fd_arr[p_fd];
  int size;

  if(me->opmap & PIPE_READER)
    size = read_pipe(me->p,buf,count);
  else
    return -EPERM;

  return size;
}


int write_pipe_new(int p_fd,void *buf,size_t count){
  struct pipe_st *me = p_fd_arr[p_fd];
  int size;
  if(me->opmap & PIPE_WRITER)
    size = write_pipe(me->p,buf,count);
  else
    return -EPERM;

  return size;
}
