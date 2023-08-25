#ifndef PROTO_H__
#define PROTO_H__

#define MSG_SIZE 1024   //单个消息最大 1024 字节

#define KEYPATH "/etc/services"
#define KEYPROJ 'a'


struct mymesg{
  long mtype;
  char mtext[MSG_SIZE];
};

#endif 
