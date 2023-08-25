#ifndef PROTO__H
#define PROTO__H

//定义 key_path和key_proj来生成唯一的key
#define KEY_PATH "/etc/services"
#define KEY_PROJ 'a'

#define PATH_MAX 1024
#define DATA_MAX 1024

#define MSG_PATH 3
#define MSG_DATA 1
#define MSG_EOT 2


//定义数据传输中需要的几种包
typedef struct msg_path_st{
  long mtype;
  char path[PATH_MAX];
}msg_path_t;

typedef struct msg_data_st{
  long mtype;
  char data[DATA_MAX];
  int len;
}msg_data_t;

typedef struct msg_eot_st{
  long mtype;
}msg_eot_t;

union s2c_st{
  long mtype; //将 mtype提取出来做判断，不提取也可以，只是每次都得用 data.p_data.mtype(data是union变量)
  msg_data_t p_data;
  msg_eot_t p_eot;
};

#endif
