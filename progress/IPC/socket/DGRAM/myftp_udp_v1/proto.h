#ifndef PROTO__H
#define PROTO__H

#define DATA_MAX 1024
#define PATH_MAX 1024

#define MSG_DATA 1
#define MSG_PATH 2
#define MSG_EOT 3
#define MSG_ACK 4


//定义几种数据包类型，包括 path data eot


typedef struct udp_data_st{
  long mtype;
  char data[DATA_MAX];
  long ackno;
  int len;
}udp_data_t;

typedef struct udp_path_st{
  long mtype;
  char path[PATH_MAX];
  long ackno;
}udp_path_t;

typedef struct udp_eot_st{
  long mtype;
  long ackno;
}udp_eot_t;

typedef struct udp_ack_st{
  long mtype;
  long ackno;
}udp_ack_t;

union udp_s2c_st{
  long mtype; //提取出来消息类型
  udp_data_t u_data;
  udp_path_t u_path;
  udp_eot_t u_eot;
  udp_ack_t u_ack;
};

#endif
