#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "my_pipe.h"

//管道的数据结构，模拟循环队列实现
struct my_pipe_st{
  char base[PIPE_SIZE]; //实际存储管道的数据
  int front; //队头
  int rear;  //队尾
  int datasize; //管道中已有数据的长度
  pthread_mutex_t mut; //互斥量
  pthread_cond_t cond; //条件变量
 //互斥量+条件变量保证数据一致性，避免竞争条件
  int r_count; //读者个数
  int w_count; //写者个数

};

  
int register_client(struct my_pipe_st *ptr,int opmap){ //注册读写管道的客户端，客户端类型通过 opmap 参数（位图）传递
  //在使用管道之前需要先注册客户端，凑齐一对读写客户端
  struct my_pipe_st *p = ptr;
  if(pthread_mutex_lock(&p->mut) != 0)
    return -1;
 
  if(opmap & PIPE_READER)
    p->r_count++;
  if(opmap & PIPE_WRITER)
    p->w_count++;

  //读者写者数量发生变化后通知阻塞在条件变量的线程
  pthread_cond_broadcast(&p->cond); 

  //在注册客户端的时候，读者和写者需要同时存在，如果不是，则等待
  while(p->r_count <= 0 || p->w_count <= 0)
    pthread_cond_wait(&p->cond,&p->mut);

  if(pthread_mutex_unlock(&p->mut) != 0)
    return -1;

  return 0;
}

int unregister_client(struct my_pipe_st *ptr,int opmap){
  //注销客户端
  struct my_pipe_st *p = ptr;
  if(pthread_mutex_lock(&p->mut) != 0)
    return -1;

  if(opmap & PIPE_READER)
    p->r_count--;
  if(opmap & PIPE_WRITER)
    p->w_count--;

  pthread_cond_broadcast(&p->cond);

  if(pthread_mutex_unlock(&p->mut) != 0)
    return -1;

  return 0;
}

//从管道中读向buf中写
static int read_byte_unlock(struct my_pipe_st *ptr,char *src,size_t count){
  int i;
  struct my_pipe_st *p = ptr;
  for(i = 0; i < count; i++){
    *src = *(p->base + p->front);
    p->front = (p->front + 1) % PIPE_SIZE;
    p->datasize--;
    //src = (src + 1) % PIPE_SIZE;   指针没有整除运算
    src++;
  }

  return 0;
}

static int write_byte_unlock(struct my_pipe_st *ptr,const char *src,size_t count){
  int i;
  struct my_pipe_st *p = ptr;
  for(i = 0; i < count; i++){
    *(p->base + p->rear) = *src;
    src++;
   // dst = (dst + 1) % PIPE_SIZE;
    p->rear = (p->rear + 1) % PIPE_SIZE;
    p->datasize++;
  }

  return 0;  
}

struct my_pipe_st *create_pipe(void){
  //成功返回管道结构指针，出错返回 NULL
  struct my_pipe_st *p = malloc(sizeof(struct my_pipe_st));
  if(p == NULL)
    return NULL; //库函数封装，不做错误输出，因为不确定会在什么地方调用库函数，如果输出到标准输出，可能在守护进程中调用，那么输出就不起作用了

  p->front = p->rear = 0;
  p->datasize = 0;

  p->r_count = p->w_count = 0;
 
  if(pthread_mutex_init(&p->mut,NULL) != 0)
    return NULL;

  if(pthread_cond_init(&p->cond,NULL) != 0)
    return NULL;

  return p;
}

int read_pipe(struct my_pipe_st *ptr,void *buf,size_t count){
  //读管道  要求管道里面有数据
  //成功返回读到的字节数，出错返回 -1,正常读完返回0      管道里面没有数据并且有写者才等待，在调用读写管道的函数前需要先注册客户端
  //读者数和写者数不能根据一次 read 或一次 write 来加
  int size;
  struct my_pipe_st *p = ptr;
  if(pthread_mutex_lock(&p->mut) != 0)
    return -1;
  
  while(p->datasize <= 0 && p->w_count > 0)  //如果管道为空并且有写者，那么读管道时等待
    pthread_cond_wait(&p->cond,&p->mut);

  if(p->datasize <= 0 && p->w_count <= 0)  //如果管道为空并且没有写者，那么关闭管道
    return 0;  //正常返回

  //两者取其小
  size = count > p->datasize ? p->datasize : count;
 
  //memcpy(buf,p->base+p->front,size); //从队头读   不能用这种方式读，因为队列是循环的，可能会从数组的尾部再读到头部
  if(read_byte_unlock(p,buf,size) != 0)
    return -1;

  //p->front = (p->front + size) % PIPE_SIZE;
  //p->datasize -= size;
  
  //通知写者
  if(pthread_cond_broadcast(&p->cond) != 0)
    return -1;

  if(pthread_mutex_unlock(&p->mut) != 0)
    return -1;
  
  return size;
}

int write_pipe(struct my_pipe_st *ptr,const void *buf,size_t count){
  int size;
  struct my_pipe_st *p = ptr;
  if(pthread_mutex_lock(&p->mut) != 0)
    return -1;
  
  while(p->datasize >= PIPE_SIZE-1 && p->r_count > 0) //如果管道满并且有读者，那么等待
    pthread_cond_wait(&p->cond,&p->mut);

  if(p->r_count <= 0)
    return -1;

  //程序走到这里，说明管道未满并且有读者,那么可以开始读数
  //两者取其小
  size = (PIPE_SIZE-1-p->datasize) > count ? count : (PIPE_SIZE-1-p->datasize); 

  //memcpy(p->base+p->rear,buf,size);  循环队列的写不能这样写
  if(write_byte_unlock(p,buf,size) != 0)
    return -1;

  p->rear = (p->rear + size) % PIPE_SIZE;
  p->datasize += size;
  //通知读者
  pthread_cond_broadcast(&p->cond);

  if(pthread_mutex_unlock(&p->mut) != 0)
    return -1;

  return size;
}


int destroy_pipe(struct my_pipe_st *ptr){
  //成功返回0，出错返回-1
  struct my_pipe_st *p = ptr;
  p->front = p->rear = 0;
  p->r_count = p->w_count = 0;
  p->datasize = 0;
  if(pthread_mutex_destroy(&p->mut) != 0)
    return -1;
  if(pthread_cond_destroy(&p->cond) != 0)
    return -1;

  free(p);
  return 0;  
}
