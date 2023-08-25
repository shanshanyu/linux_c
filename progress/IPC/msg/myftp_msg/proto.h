#ifndef PROTO_H__
#define PROTO_H__

#define KEYPATH "/opt"
#define KEYPROJ  'a'

#define PATHMAX 1024
#define DATAMAX 1024

//3 种类型的包，一种 path 包，客户端发送，服务端接收。data 包和 eot（end of translation）包，服务端发送客户端接收

enum{
  MSG_PATH = 1,
  MSG_DATA,
  MSG_EOT
};


typedef struct msg_path_st{
  long mtype;  //must be MSG_PATH
  char path[PATHMAX];
}msg_path_t;


typedef struct msg_data_st{
  long mtype;  //must be MSG_DATA
  int datalen; //为了解决空洞文件的问题，加入 datalen
  char data[DATAMAX];
}msg_data_t;

typedef struct msg_eot_st{
  long mtype;  //must be MSG_EOT
}msg_eot_t;

union s2c_st{  //eot 和 data 包第一个字段都是 long mtype,将 mtype 提取出来做判断。
  long mtype;
  msg_data_t data;
  msg_eot_t eot;
};

#endif
