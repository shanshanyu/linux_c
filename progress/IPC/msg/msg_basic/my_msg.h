#ifndef MY_MSG_H__
#define MY_MSG_H__

#define KEYPATH  "/etc/services"
#define KEYPROJ 'a'
#define MSG_SIZE 512

struct mymsg_st{
  long mtype;  //正整数的消息类型
  char mtext[MSG_SIZE];  //消息数据
};

#endif
