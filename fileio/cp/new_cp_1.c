#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFSIZE 1024



int main(int argc,const char *argv[]){
  //判断参数传递是否合理
  if(argc != 3){
    fprintf(stderr,"Usage:%s src_file dst_file\n",argv[0]);
    exit(1);
  }
  
  int src_fd,dst_fd;
  ssize_t rd_ret,wr_ret;  
  char buf[BUFSIZE]; //设置缓冲区
  //int pos; //写的定位器
  //打开文件
  src_fd = open(argv[1],O_RDONLY);
  if(src_fd == -1){
     fprintf(stderr,"open src_file failed\n");
     exit(1);
  }
  
  dst_fd = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644);
  if(dst_fd == -1){
     fprintf(stderr,"open dst_file failed\n");
     exit(1);
  }

 //复制文件
 while(1){
   rd_ret = read(src_fd,buf,BUFSIZE);
   //如果读错，程序退出
   if(rd_ret == -1){
     if(errno == EINTR)
      continue;
     else{
       fprintf(stderr,"read failed\n");
       exit(1);
     }
   }
   //如果读到文件尾，退出循环
   if(rd_ret == 0)
     break;
   
   //pos = 0;
   /*do{
     wr_ret = write(dst_fd,buf,rd_ret);
     if(wr_ret == -1){
       if(errno == EINTR)
         continue;
       else{
         fprintf(stderr,"write failed\n");
         exit(1);
       }
     }
     pos += wr_ret;

   }while(pos < rd_ret);
   
   */

  while(rd_ret > 0){
    wr_ret = write(dst_fd,buf,rd_ret);  //这里写的不对，处理普通的文件没有问题，如果处理终端、管道等慢速设备被信号打断后，这里的逻辑就不对了
    if(wr_ret == -1){
       if(errno == EINTR)
         continue;
       else{
         fprintf(stderr,"write failed\n");
         exit(1);
       }
    }
    rd_ret -= wr_ret;
    printf("rd_ret = %d\n",rd_ret);
  }

  }
  close(src_fd);
  close(dst_fd);
 
  exit(0);
}
