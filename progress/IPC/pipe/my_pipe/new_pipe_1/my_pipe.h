#ifndef MY_PIPE_H__
#define MY_PIPE_H__

#include <unistd.h>

#define PIPE_SIZE 1024
#define PIPE_READER 0x00000001
#define PIPE_WRITER 0x00000002

struct my_pipe_st;

struct my_pipe_st *create_pipe(void);
int read_pipe(struct my_pipe_st*,void *,size_t);
int write_pipe(struct my_pipe_st*,const void *,size_t);
int destroy_pipe(struct my_pipe_st *);

int unregister_client(struct my_pipe_st*,int);
int register_client(struct my_pipe_st*,int);

#endif
