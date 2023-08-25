#include <stdio.h>

#include <stdlib.h>
#define BUFSIZE 1024

//fclose 丢掉输入缓冲，刷新输出缓冲
//这个输入缓冲和输出缓冲都是这个流的，不是标准的输入和输出流缓冲
//标准输入和标准输出是行缓冲的


int main(int argc, char **argv)

{

  int a;
  char buf[1024];
  FILE *fp = NULL;
  fp = fopen("/tmp/out","w+");
  if(fp == NULL){
    perror("fopen()");
    exit(1);
  }

  fprintf(stdin,"hello");  //标准输入是键盘而不是显示器
  //数据写到文件中
  
  scanf("%s",buf);
  putchar(a);
   /*if(fgets(buf,BUFSIZE,fp) == NULL){
     if(feof(fp)){
       fprintf(stderr,"end of file\n");
     }else
       fprintf(stderr,"error\n");
   }
  */
  // fclose(fp);
   //sleep(5);


   //puts(buf); 
   fclose(fp);
   return 0;

}
