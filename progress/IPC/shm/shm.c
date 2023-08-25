#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>

#define KEYPATH "/etc/services"
#define KEYPROJ 'a'
#define SHMSIZE 1024

int main(){
  key_t key;
  pid_t pid;
  int shmid;
  char *ptr;  

  key = ftok(KEYPATH,KEYPROJ);
  if(key < 0){
    perror("ftok()");
    exit(1);
  }

  //创建共享存储
  shmid = shmget(key,SHMSIZE,IPC_CREAT|IPC_EXCL|0644);
  if(shmid < 0){
    perror("shmget()");
    exit(1);
  }

  ptr = shmat(shmid,0,0);
  if(ptr == (void *)-1){
    perror("shmat()");
    exit(1);
  }
  //shm init over
  pid = fork();
  if(pid < 0){
    shmctl(shmid,IPC_RMID,NULL);
    exit(1);
  }else if(pid == 0){
    //child
    strcpy(ptr,"hello");
    shmdt(ptr);
    exit(0);

  }else{
  //parent
    wait(NULL);
    puts(ptr);

  }
  
  shmdt(ptr);
  shmctl(shmid,IPC_RMID,NULL);
  exit(0);

}
