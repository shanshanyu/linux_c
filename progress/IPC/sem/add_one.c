#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>

#define FILENAME "/tmp/out"
#define PROCNUM 20
#define BUFSIZE 1024

union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};

static int semid;

static void P(){
  //信号量 -
  struct sembuf op;
  op.sem_num = 0;
  op.sem_op = -1;
  op.sem_flg = 0;

  //考虑被信号打断或者非阻塞情况
  while(semop(semid,&op,1) < 0){
    if(errno != EINTR || errno != EAGAIN){
      perror("semop()");
      exit(1); 
    }
  } 
}

static void V(){
  //信号量 +
  struct sembuf op;
  op.sem_num = 0;
  op.sem_op = 1;
  op.sem_flg = 0;

  if(semop(semid,&op,1) == -1){
    perror("semop()");
    exit(1);
  }

}

static void add_one(){
  FILE *fp;
  char buf[BUFSIZE];
 
  P();

  //打开文件
  fp = fopen(FILENAME,"r+");
  if(fp == NULL){
    fprintf(stderr,"fopen() failed\n");
    exit(1);
  }

  //如果 fgets 返回 NULL，测试是到达文件结尾还是出错
  if(fgets(buf,BUFSIZE,fp) == NULL){
    if(ferror(fp)){
      fprintf(stderr,"fgets() failed\n");
      exit(1);
    }
  }

  if(fseek(fp,0,SEEK_SET) == -1){
    perror("fseek()");
    exit(1);
  }
  sleep(1);
  //格式化输出
  fprintf(fp,"%d",atoi(buf)+1);
  fflush(fp);
   
  //关闭文件 
  fclose(fp);

  V();

}


int main(){
  pid_t pid;
  int i;
  //创建信号量
  semid = semget(IPC_PRIVATE,1,0644);
  if(semid < 0){
    perror("semget()");
    exit(1);
  }
  //初始化信号量
  union semun arg;
  arg.val = 1;
  if(semctl(semid,0,SETVAL,arg) < 0){
    perror("semctl()");
    exit(1);
  }


  //创建进程，子进程执行 add_one
  for(i = 0; i < PROCNUM; i++){
    pid = fork();
    if(pid < 0){
      perror("fork()");
      exit(1);
    }   
    if(pid == 0){
      add_one();
      exit(0);
    }
  }

  for(i = 0; i < PROCNUM; i++){
    wait(NULL);
  }

  if(semctl(semid,0,IPC_RMID) < 0){
    perror("semctl()");
    exit(1);
  }

  exit(0);
}
