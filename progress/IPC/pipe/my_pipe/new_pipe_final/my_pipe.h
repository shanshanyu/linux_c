#ifndef MY_PIPE_H__
#define MY_PIPE_H__

#include <unistd.h>

#define PIPE_SIZE 1024
#define PIPE_READER 0x00000001
#define PIPE_WRITER 0x00000002

typedef void my_pipe_t;

my_pipe_t *create_pipe(void);
int read_pipe(my_pipe_t *,void *,size_t);
int write_pipe(my_pipe_t *,const void *,size_t);
int destroy_pipe(my_pipe_t *);

int unregister_client(my_pipe_t *,int);
int register_client(my_pipe_t *,int);

#endif
