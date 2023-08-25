#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define BUFSIZE 1024

//对状态机的每个状态做枚举
enum FSM_STATE{
  STATE_R,
  STATE_W,
  STATE_Ex,
  STATE_T //termination
};

typedef struct fsm_st{
  int fd_src;
  int fd_dst;
  enum FSM_STATE state; //状态机的状态
  int rd_ret;
  char buf[BUFSIZE];
  int pos; //记录写的位置,如果本次读的不够，状态机会重新推到写状态，之前写的进度需要有保存。
  char *err_str; //记录错误的信息
}fsm_st;

//设置文件状态标志
static int set_fl(int fd,int flags){
  //flags 表示需要打开的标志
  int val;
  val = fcntl(fd,F_GETFL);
  if(val == -1){
    fprintf(stderr,"fcntl failed\n");
    exit(1); //这里应该返回-1，交给主调程序去判断
  }

  if(fcntl(fd,F_SETFL,val|flags) == -1){
    fprintf(stderr,"fcntl failed\n");
    exit(1);
  }
  printf("set flag success\n");
  return val; //获得之前的文件状态标志之后恢复
}

//推状态机
static void fsm_driver(fsm_st *fsm){
  int wr_ret;
  //根据 fsm->state 的状态来推状态机
  switch(fsm->state){
    case STATE_R:
      fsm->rd_ret = read(fsm->fd_src,fsm->buf,BUFSIZE);
      if(fsm->rd_ret < 0){
        //如果是假错继续读
        if(errno == EAGAIN)
          fsm->state = STATE_R;
        //如果是真错则退出
        else{
          fsm->err_str = "read failed\n"; 
          fsm->state = STATE_Ex;
        }
      //如果读到了数据则接着写，把状态推到写
      }else if(fsm->rd_ret > 0){
        fsm->pos = 0;
        fsm->state = STATE_W;
      //读到文件末尾，程序终止
      }else{
        fsm->state = STATE_T;
      }
      printf("read over\n");
      break;
    case STATE_W:
      wr_ret = write(fsm->fd_dst,fsm->buf+fsm->pos,fsm->rd_ret-fsm->pos);
      if(wr_ret < 0){
        if(errno == EAGAIN){
          printf("again\n");
          fsm->state = STATE_W;
        }
        else{
          fsm->err_str = "write failed\n";
          fsm->state = STATE_Ex;
        }
      //写了数据
      }else{
        //数据没写完
        if(fsm->pos + wr_ret < fsm->rd_ret){
          fsm->pos += wr_ret;
          fsm->state = STATE_W;
          printf("write remain\n");
        //数据写完了
        }else{
          //数据写完后需要重新设置文件位置指针,在这个位置设置没有问题，因为最终进入写之前pos都被置为0
          //不过这个pos应该在读完之后再做初始化
          //fsm->pos = 0;
          fsm->state = STATE_R;
        }
      }
      printf("write over\n");
      break;
    case STATE_Ex:
      //打印错误信息
      perror(fsm->err_str);
      fsm->state = STATE_T;
      break;
    case STATE_T:
      printf("state_t\n");
      break;
    default:
      printf("goto default\n");
      break;
  }
  

}


void relay(int fd_l,int fd_r){ //分别用 fd_l 表示左，fd_r 表示右
  int fdl_save,fdr_save;
  //从左复制到右，从右复制到左，每边需要一个状态机
  fsm_st *fsm_node1 = malloc(sizeof(fsm_st));
  if(fsm_node1 == NULL){
    fprintf(stderr,"malloc failed,Exit..\n");
    exit(1);
  }

  fsm_st *fsm_node2 = malloc(sizeof(fsm_st));
  if(fsm_node2 == NULL){
    fprintf(stderr,"malloc failed,Exit..\n");
    exit(1);
  } 
  //两个状态机创建完成
   
  //fdl_save = set_fl(fd_l,O_NONBLOCK);
  //fdr_save = set_fl(fd_r,O_NONBLOCK);

  //下面初始化状态机
  
  fsm_node1->fd_src = fd_l;
  fsm_node1->fd_dst = fd_r;
  fsm_node1->state = STATE_R; //状态机的初始状态是读态
 // fsm_node1->pos = 0; //写状态的位置
 
  fsm_node2->fd_src = fd_r;
  fsm_node2->fd_dst = fd_l;
  fsm_node2->state = STATE_R;//状态机的初始状态是读态
  //fsm_node2->pos = 0;//写状态的位置

  //两个状态机初始化完成
  //如果两个状态机中的任意一个状态机不是终止状态，那么就推状态机
  while(fsm_node1->state != STATE_T || fsm_node2->state != STATE_T){
    fsm_driver(fsm_node1);
    fsm_driver(fsm_node2);
  }
  
  //恢复文件状态标志
 // set_fl(fd_l,fdl_save);
 // set_fl(fd_r,fdr_save);
}
