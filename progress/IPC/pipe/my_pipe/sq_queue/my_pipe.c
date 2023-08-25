#include "my_pipe.h"
#include <stdio.h>
#include <stdlib.h>

#define PIPE_SIZE 1024 //管道大小

typedef int elemtype;

//一个循环队列,不能算管道，只能说操作方式和管道类似 FIFO
typedef struct my_pipe_st{
  elemtype *e;
  int front; //队头
  int rear; //队尾
}my_pipe_t;


int create_pipe(my_pipe_t *p){
  //
  p->e  = malloc(sizeof(elemtype)*PIPE_SIZE);
  if(!p->e){
    fprintf(stderr,"malloc() failed\n");
    return -1;
  }
  p->front = 0;
  p->rear = 0;
  return 0;
}

int destroy_pipe(my_pipe_t *p){
  //
  free(p->e);
  p->front = 0;
  p->rear = 0;
  return 0;
}

int en_pipe(my_pipe_t *p,elemtype e){
  //写数据
  if((p->rear + 1) % PIPE_SIZE == p->front){
    fprintf(stderr,"pipe full\n");
    return -1;
  }
  p->e[p->rear] = e;
  p->rear = (p->rear + 1) % PIPE_SIZE;
  return 0; 
}

int de_pipe(my_pipe_t *p,elemtype *e){
  //读数据
  if(p->rear == p->front){
    fprintf(stderr,"pipe empty\n");
    return -1; 
  }
  *e = p->e[p->front];
  p->front = (p->front + 1) % PIPE_SIZE;
  return 0; 
}






int main(){
  my_pipe_t p;
  elemtype e;
  if(create_pipe(&p) == -1){
     fprintf(stderr,"create_pipe failed\n");
     exit(1);
  }
  if(en_pipe(&p,3) == -1){
    fprintf(stderr,"en_pipe failed\n");
    exit(1);
  }
  if(de_pipe(&p,&e) == -1){
     fprintf(stderr,"de_pipe failed\n");
     exit(1);
  }
  printf("e = %d\n",e);
  if(destroy_pipe(&p) == -1){
    fprintf(stderr,"destory_pipe failed\n"); 
    exit(1);

  }
  return 0;
}
