#ifndef  PROTO__H
#define PROTO__H

#define NAMESIZE 11

struct msg_st{
  char name[NAMESIZE];
  uint32_t math;
  uint32_t chinese;
}__attribute((packed));

#endif
