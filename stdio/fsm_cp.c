#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFSIZE 1024

enum fsm_state{
 STATE_R,
 STATE_W,
 STATE_Ex,
 STATE_T
};

typedef struct fsm_st{
  enum fsm_state state;
  int src_fd;
  int dst_fd;
  int r_count;
  int w_count;
  char buf[BUFSIZE];
}fsm_st;

void fsm_driver(fsm_st *st){
  switch(st->state){
    case STATE_R:
      st->r_count = read(st->src_fd,st->buf,BUFSIZE);
      if(st->r_count < 0)
        st->state = STATE_Ex;
      else if(st->r_count == 0)
        st->state = STATE_T;
      else
        st->state = STATE_W;
      break;

    case STATE_W:
      st->w_count = write(st->dst_fd,st->buf,st->r_count);
      if(st->w_count < 0)
        st->state = STATE_Ex;
      else if(st->w_count != st->r_count)
        st->state = STATE_Ex;
      else
        st->state = STATE_R;
      break;  

    case STATE_Ex:
      perror("read or write failed");
      st->state = STATE_T;
      break;

    case STATE_T:
      ;

    default:  
      fprintf(stderr,"fsm state error,please cheak\n");
      st->state = STATE_T;
  }

}

int main(int argc,const char *argv[]){
  if(argc != 3){
    printf("argc = %d\n",argc);
    fprintf(stderr,"Usage:%s srcfile dstfile\n",argv[0]);
    exit(0);
  }

  fsm_st st;
  st.state = STATE_R;
  //open src_file
  st.src_fd = open(argv[1],O_RDONLY);
  if(st.src_fd == -1){
    perror("open src_file failed");
    exit(1);
  }
  //open dst_file
  st.dst_fd = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644);
  if(st.dst_fd == -1){
    perror("open dst_file failed");
    exit(1);
  }

 while(st.state != STATE_T)
   fsm_driver(&st); 

  exit(0);
}
