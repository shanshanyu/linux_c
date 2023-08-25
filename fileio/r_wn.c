#include <stdio.h>
#include <stdlib.h>




int readn(int fd,char *buf,int n){
  int  n_left,n_read;
  n_left = n;
  while(1){
   n_read = read(fd,buf,n);
   if(n_read < 0){
     if(n_left == n)
       return -1; //一个字节都没读到
     else
       break;
   }else if(n_read  == 0)
     break;
    
   n_left -= n_read;
   n 
      
    
  }
  
  
}
