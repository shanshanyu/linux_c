#include "anytimer.h"

void f1(void *arg){
  printf("f1 print %s\n",arg);
}

void f2(void *arg){
  printf("f2 print %s\n",arg);
}


int main(){

  timer *t1,*t2,*t3;

  t1 = init_timer(5,f1,"aaa");
  t2 = init_timer(2,f2,"bbb");
  t3 = init_timer(7,f1,"ccc");

  while(1)
   pause();

}
