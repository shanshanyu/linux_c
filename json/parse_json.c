#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

/*
 *author:yss
 *data: 2023-03-=28
 *v1.0
 *
 *用来解析json数据，将json数据中的一些字段去掉，再加上一些字段
 */

#define BUFSIZE 10485760

int main(int argc,const char *argv[]){
  FILE *fp = NULL;  
  FILE *new_fp = NULL;
  char *buf = NULL;
  cJSON *json = NULL;

  if(argc != 2){
    fprintf(stderr,"Usage: %s srcfile\n",argv[0]);
    exit(1);
  }

  //堆上开辟空间存放临时json数据
  buf = malloc(BUFSIZE);
  if(buf == NULL){
    fprintf(stderr,"malloc failed\n");
    exit(1);
  }
  
  //打开文件
  fp = fopen(argv[1],"r");
  if(fp == NULL){
    fprintf(stderr,"fopen %s filed\n",argv[1]);
    exit(1);
  }
  
  //todo:后面可以通过参数指定要保存的文件
  new_fp = fopen("/tmp/out1","w");
  if(new_fp == NULL){
    fprintf(stderr,"fopen() failed");
    exit(1);
  }

  memset(buf,'\0',BUFSIZE);
  while(fgets(buf,BUFSIZE,fp)){
  //从文件中读取一行 json 数据
    json = cJSON_Parse(buf);
    if(json  == NULL){
      fprintf(stderr,"%s is not valid json str\n",buf);
      continue;
    }
  //json数据处理
  //'error_type','error_reason','extractor','_flush_time','recv_time'  这些字段去掉,加上一个 time_free 属性
    cJSON_DeleteItemFromObject(json,"error_type");
    cJSON_DeleteItemFromObject(json,"error_reason");
    cJSON_DeleteItemFromObject(json,"extractor");
    cJSON_DeleteItemFromObject(json,"_flush_time");
    cJSON_DeleteItemFromObject(json,"recv_time");
    cJSON_AddBoolToObject(json,"time_free",1);
  //json数据写入到新文件
    memset(buf,'\0',BUFSIZE);
    strcpy(buf,cJSON_PrintUnformatted(json));
    fputs(buf,new_fp);
    fputc('\n',new_fp);
  }
  if(feof(fp)){
     fprintf(stdout,"json ok");
   }else if(ferror(fp)){
     fprintf(stderr,"json failed");
   }

   //释放空间
   cJSON_Delete(json);
   free(buf);

  //关闭文件
   fclose(fp);
   fclose(new_fp);
  exit(0);
}
