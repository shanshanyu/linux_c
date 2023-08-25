#ifndef OP_PIPE_H__
#define OP_PIPE_H__

#include <unistd.h>
int open_pipe(int,int);
int close_pipe(int);
int read_pipe_new(int,void *,size_t);
int write_pipe_new(int,void *,size_t);


#endif
