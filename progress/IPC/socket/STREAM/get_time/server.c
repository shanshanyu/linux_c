#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <syslog.h>
#include <sys/file.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define LOCK_FILE "/var/tmp/server.pid"
#define LOCK_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

#define BUFSIZE 1024

void daemonize(){
  pid_t pid;
  struct rlimit limit;
  int i;
  int fd0,fd1,fd2,lock_fd;
  char buf[BUFSIZE];

  //fork创建子进程
  pid = fork();
  if(pid < 0){
    perror("fork()");
    exit(1);
  }else if(pid > 0)
    exit(0);

  //创建会话
  if(setsid() < 0){
    perror("setsid()");
    exit(1);
  }

  //设置程序的文件模式创建屏蔽字  默认为 0002 设置为0，给与最大权限
  umask(0);

  //更改进程的工作目录
  if(chdir("/") < 0){
    perror("chdir()");
    exit(1);
  }

  //关闭不必要的文件描述符
  if(getrlimit(RLIMIT_NOFILE,&limit) < 0){
    perror("getrlimit()");
    exit(1);
  }

  if(limit.rlim_max == RLIM_INFINITY)
    limit.rlim_max = 1024;

  for(i = 0; i < limit.rlim_max; i++)
    close(i);


  fd0 = open("/dev/null",O_RDWR);
  fd1 = dup(fd0);
  fd2 = dup(fd0);

  //日志文件打印到系统目录
  openlog("myftp_msg_v1",LOG_CONS,LOG_USER);
  if(fd0 != 0 && fd1 != 1 && fd2 != 2){
    syslog(LOG_ERR,"stdin stdout stderr redirect failed\n");
    exit(1);
  }

  //创建单实例守护进程
  lock_fd = open(LOCK_FILE,O_RDWR|O_CREAT|O_TRUNC,LOCK_MODE);
  if(lock_fd < 0){
    syslog(LOG_ERR,"open():%m");
    exit(1);
  } 

   //锁定文件
  if(flock(lock_fd,LOCK_EX|LOCK_NB) < 0){
    syslog(LOG_ERR,"flock():%m");
    exit(1);
  }

  sprintf(buf,"%d",getpid());
  //将 pid 写入文件中
  if(write(lock_fd,buf,strlen(buf)+1) < 0){
    syslog(LOG_ERR,"write():%m");
    exit(1);
  }
 
  syslog(LOG_INFO,"daemon intial success.\n");
}


//返回当前的服务器时间给客户端
void serve_job(int sockfd){
  time_t t;
  struct tm *tm;
  char buf[BUFSIZE];

  t = time(NULL);
  tm = localtime(&t);

  strftime(buf,BUFSIZE,"%Y%m%d-%H%M%S",tm);

  write(sockfd,buf,BUFSIZE);  
}




int main(){
  pid_t pid;
  int sockfd,new_sockfd;
  struct sockaddr_in laddr;

  //服务端进程初始化成守护进程
  daemonize();

  //创建socket
  sockfd = socket(AF_INET,SOCK_STREAM,0);
  if(sockfd < 0){
    syslog(LOG_ERR,"socket():%m");
    exit(1);
  }  
  
  //socket与地址绑定
  laddr.sin_port = htons(2888);
  laddr.sin_family = AF_INET;
  inet_pton(AF_INET,"172.19.229.90",&laddr.sin_addr);

  if(bind(sockfd,(void *)&laddr,sizeof(laddr)) < 0){
    syslog(LOG_ERR,"bind():%m");
    exit(1);
  }

  //服务端宣告愿意接收连接
  if(listen(sockfd,128) < 0){
    syslog(LOG_ERR,"listen():%m");
    exit(1);
  }

  while(1){
    new_sockfd = accept(sockfd,NULL,NULL);
    if(new_sockfd < 0){
      syslog(LOG_ERR,"accept():%m");
      exit(1);
    }
    
    pid = fork();
    if(pid < 0){
      syslog(LOG_ERR,"fork():%m");
      exit(1);
    }else if(pid == 0){
      close(sockfd);
      serve_job(new_sockfd);
      close(new_sockfd);
      exit(0);
    }
    close(new_sockfd);
  }

  exit(0);
}
