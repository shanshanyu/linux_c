#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

#define BUFSIZE 1024
#define SERVER_STRING "Server: yss httpd/0.1.0\r\n"

#define STDIN 0
#define STDOUT 1
#define STDERR 2

// my http server

void err_msg(const char *s){  //对于有 errorno 报错的函数，可以用这个函数来打印错误消息并退出
  perror(s);
  exit(1);
}

int startup(uint16_t port){ //listenfd 的初始化
  int listenfd;
  int on = 1;
  struct sockaddr_in servaddr;
  listenfd = socket(AF_INET,SOCK_STREAM,0); //创建 socket
  if(listenfd == -1)
    err_msg("startup socket() failed");

  //初始化 servaddr
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) == -1) //打开地址重用
    err_msg("startup setsockopt() failed\n");
  
  if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1)
    err_msg("startup bind() failed\n");

  if(listen(listenfd,128) == -1)
    err_msg("startup listen() failed\n");

  return listenfd; 
}

//读取一行，每行的结尾是 \n 加上字符串的结束符 \0
int get_line(int sock,char *buf,int size){
  int i = 0;
  char c = '\0';
  int n;
  while((i < size-1) && (c != '\n')){
    n = recv(sock,&c,1,0); //每次读取一个字节
    if(n > 0){
      if(c == '\r'){  //如果接收到 \r,得判断下一个是不是\n,如果是\n，需要把\n 取出来
        n = recv(sock,&c,1,MSG_PEEK);
        if((n>0) && (c == '\n'))
          recv(sock,&c,1,0);
        else
          c = '\n'; //如果  
      }
      buf[i] = c;
      i++;
    }else
       c = '\n'; 
  }
  buf[i] = '\0';
  return i;
}

void unimplemented(int client){
  char buf[BUFSIZE];
  sprintf(buf,"HTTP/1.0 501 Method Not Implemented\r\n");
  send(client,buf,strlen(buf),0);
  sprintf(buf,SERVER_STRING); //http 响应头中的server 字段
  send(client,buf,strlen(buf),0);
  sprintf(buf,"Content-Type: text/html\r\n"); //http 响应头中的 content-type
  send(client,buf,strlen(buf),0);
  sprintf(buf,"\r\n");
  send(client,buf,strlen(buf),0);
  sprintf(buf,"<HTML><HEAD><TITLE>Method Not Implemented\r\n");
  send(client,buf,strlen(buf),0);
  sprintf(buf,"</TITLE></HEAD>\r\n");
  send(client,buf,strlen(buf),0);
  sprintf(buf,"<BODY><P>HTTP request method not supported.\r\n");
  send(client,buf,strlen(buf),0);
  sprintf(buf,"</BODY></HTML>\r\n");
  send(client,buf,strlen(buf),0);
}

void not_found(int client){ //404 not found
  char buf[BUFSIZE];
  sprintf(buf,"HTTP/1.0 404 NOT FOUND\r\n");
  send(client,buf,sizeof(buf),0);
  sprintf(buf,SERVER_STRING);
  send(client,buf,sizeof(buf),0);
  sprintf(buf,"Content-Type: text/html\r\n");
  send(client,buf,sizeof(buf),0);
  sprintf(buf,"\r\n");
  send(client,buf,sizeof(buf),0);
  sprintf(buf,"<HTML><TITLE>Not Found</TITLE>\r\n");
  send(client,buf,sizeof(buf),0);
  sprintf(buf,"<BODY><P>The server could not fulfill\r\n");
  send(client,buf,sizeof(buf),0);
  sprintf(buf,"your request becasuse the resource specified\r\n");
  send(client,buf,sizeof(buf),0);
  sprintf(buf,"is unavailable or nonexistent.\r\n");
  send(client,buf,sizeof(buf),0);
  sprintf(buf,"</BODY></HTML>\r\n");
  send(client,buf,sizeof(buf),0);

}

void headers(int client,const char *filename){ //200 ok
  char buf[BUFSIZE];
  
  strcpy(buf,"HTTP/1.0 200 OK\r\n");
  send(client,buf,strlen(buf),0);
  strcpy(buf,SERVER_STRING);
  send(client,buf,strlen(buf),0);
  strcpy(buf,"Content-Type: text/html\r\n");
  send(client,buf,strlen(buf),0);
  strcpy(buf,"\r\n");
  send(client,buf,strlen(buf),0);

}

void cat(int client,FILE *resource){
  char buf[BUFSIZE];
  
  fgets(buf,sizeof(buf),resource); //读取一行
  while(!feof(resource)){   //读取文件中的数据并输出到套接字
    send(client,buf,strlen(buf),0);
    fgets(buf,sizeof(buf),resource);
  }
}

void serve_file(int client,const char *filename){
  FILE *resource = NULL;
  char buf[BUFSIZE];
  int numchars = 1;

  while((numchars>0) && strcmp("\n",buf)) 
    numchars = get_line(client,buf,sizeof(buf));

  resource = fopen(filename,"r");
  if(resource == NULL)
    not_found(client); //404 not found
  else{
    headers(client,filename); //200 ok
    cat(client,resource);
  }
}

void bad_request(int client){
  char buf[BUFSIZE];
  sprintf(buf,"HTTP/1.0 400 BAD REQUEST\r\n");
  send(client,buf,sizeof(buf),0);
  sprintf(buf,"Content-type: text/html\r\n");
  send(client,buf,sizeof(buf),0);
  sprintf(buf,"\r\n");
  send(client,buf,sizeof(buf),0);
  sprintf(buf,"<P>Your brower send a bad request,");
  send(client,buf,sizeof(buf),0);
  sprintf(buf,"such as a POST without a Content-Length.\r\n");
  send(client,buf,sizeof(buf),0);

}

void cannot_execute(int client){
  char buf[BUFSIZE];
  
  sprintf(buf,"HTTP/1.0 500 Internal Server Error\r\n");
  send(client,buf,strlen(buf),0);
  sprintf(buf,"Content-Type: text/html\r\n");
  send(client,buf,strlen(buf),0);
  sprintf(buf,"\r\n");
  send(client,buf,strlen(buf),0);
  sprintf(buf,"<P>Error prohibited CGI execution.\r\n");
  send(client,buf,strlen(buf),0);

}

void execute_cgi(int client,const char *path,const char *method,const char *query_string){
  char buf[BUFSIZE];
  int content_length = -1;  
  
  int cgi_input[2];
  int cgi_output[2];

  int i;
  int status; //回收子进程的状态

  char c;

  int numchars;

  pid_t pid;

  buf[0] = 'A';
  buf[1] = '\0';

  if(strcasecmp(method,"GET") == 0)
    while((numchars > 0) && strcmp("\n",buf))
      numchars = get_line(client,buf,sizeof(buf));
  else if(strcasecmp(method,"POST") == 0){
    numchars = get_line(client,buf,sizeof(buf));
    while((numchars > 0) && strcmp("\n",buf)){
      buf[15] = '\0';
      if(strcasecmp(buf,"Content-Length:") == 0)
        content_length = atoi(&buf[16]);
      numchars = get_line(client,buf,sizeof(buf));
    }
   
    if(content_length == -1){
      bad_request(client);
      return ;
    }
  }else{
    ;
  }

  if(pipe(cgi_output) < 0){
    cannot_execute(client);
    return ;
  }
  
  if(pipe(cgi_input) < 0){
    cannot_execute(client);
    return ;
  }

  if((pid = fork()) < 0){ //创建子进程
    cannot_execute(client);
    return ;
  }


  
  if(pid == 0){ //子进程
    char meth_env[255];
    char query_env[255];
    char length_env[255];
    
    
    dup2(cgi_output[1],STDOUT);
    dup2(cgi_input[0],STDIN);  //到这里后每个管道上面有 5 根线
    close(cgi_output[0]); //关闭子进程的cgi_output读
    close(cgi_input[1]); //关闭子进程cgi_input的写
   
    sprintf(meth_env,"REQUEST_METHOD=%s",method);
    putenv(meth_env); //设置环境变量

    if(strcasecmp(method,"GET") == 0){
      sprintf(query_env,"QUERY_STRING=%s",query_string);
      putenv(query_env);
    }else{ //post 方法
      sprintf(length_env,"CONTENT_LENGTH=%d",content_length);
      putenv(length_env);
    }

    execl(path,NULL);
    exit(0);
  }else{  //父进程
    close(cgi_output[1]); 
    close(cgi_input[0]);
    
    sprintf(buf,"HTTP/1.0 200 OK\r\n");  //父子进程都会执行下面这两行
    send(client,buf,strlen(buf),0);

 
    if(strcasecmp(method,"POST") == 0)
      for(i = 0; i < content_length; i++){
        recv(client,&c,1,0);
        write(cgi_input[1],&c,1);
      }

    while(read(cgi_output[0],&c,1) > 0)
      send(client,&c,1,0);

    close(cgi_output[0]);
    close(cgi_input[1]);
    waitpid(pid,&status,0);
  } 

}

void *accept_request(void *arg){
  int connfd = *(int *)arg;  //pthread_create 传递过来的参数
  char buf[BUFSIZE];  
  char method[255];
  char url[255];
  char path[512];

  size_t numchars; 
  size_t i,j;
  char *query_string = NULL;

  struct stat st; //定义stat函数需要的结构体,保存文件的属性信息

  int cgi = 0; //如果使用cgi，把cgi设置成1

  numchars = get_line(connfd,buf,BUFSIZE); //从 connfd 中读取一行
  
  while(!isspace(buf[i]) && (i < sizeof(method)-1)){  //buf[i] 不是空白符，并且 i 小于 255
    method[i] = buf[i];
    i++;
  }
  
  j = i; 
  //j 这里为什么要保存 i 的值？
  //j 记录保留的进度
  method[i] = '\0';

  if(strcasecmp(method,"GET") && strcasecmp(method,"POST")){ //strcasecmp相等返回0，这个比大使表示不等于GET也不等于POST
    unimplemented(connfd);
    return NULL;
  }
  
  if(strcasecmp(method,"POST") == 0) //打开cgi
    cgi = 1;   
  
  i = 0; //i在复制字符串的时候被用过了，这里重新设置为0
  
  while(isspace(buf[j]) && (j < numchars)) //跳过空白字符
    j++; 

  while(!isspace(buf[j]) && (i < sizeof(url)-1) && (j < numchars)){ //复制 url
    url[i] = buf[j];
    i++;
    j++;
  }

  url[i] = '\0';
  printf("url= %s\n",url);

  if(strcasecmp(method,"GET") == 0){
    query_string = url;
    while((*query_string != '?') && (*query_string != '\0')) //query_string 跳过正常字符
      query_string++;
    
    if(*query_string == '?'){  //如果 url 带参数
      cgi = 1;
      *query_string = '\0';
      query_string++;
    }   
  }

  sprintf(path,"htdocs%s",url);
  if(path[strlen(path)-1] == '/'){  //倒数第二个字符
    strcat(path,"index.html");
  }

  if(stat(path,&st) == -1){
    while((numchars > 0) && strcmp("\n",buf)) //这一行的作用是跳过请求头
      numchars = get_line(connfd,buf,sizeof(buf));
    not_found(connfd); //404 not found
  }else{
    if((st.st_mode & S_IFMT) == S_IFDIR)
      strcat(path,"/index.html");
    if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH)) //如果文件有执行权限，打开cgi
      cgi = 1;

    if(!cgi)  //如果cgi是关的，执行如下函数
      serve_file(connfd,path);
    else
      execute_cgi(connfd,path,method,query_string);
  }

  close(connfd); //关闭连接到客户端的套接字

  return NULL;
}

int main(){
  int listenfd,connfd; //listenfd 是服务端的socketfd，connfd是accept返回的，连接到客户端socket的socketfd
  struct sockaddr_in clientaddr;
  socklen_t clientaddr_len = sizeof(clientaddr);
  uint16_t port = 4000; //HTTP服务运行端口
  pthread_t tid;
  
  listenfd = startup(port);
  //startup
  
  printf("httpd server running on port %d\n",port);
  
  while(1){
    connfd = accept(listenfd,(struct sockaddr *)&clientaddr,&clientaddr_len);
    if(connfd == -1)
      err_msg("accpet() failed");
   
    if(pthread_create(&tid,NULL,accept_request,(void *)&connfd) != 0){
      perror("pthread_create()");
      exit(1); 
    }
    
  }

  close(listenfd); //关闭socket


}
