#ifndef MY_SEM_H__
#define MY_SEM_H__

struct st_sem{
  int sem_num;
  pthread_mutex_t mut;
  pthread_cond_t cond;
};

struct st_sem *my_sem_init(int sem_num);

int my_sem_add(struct st_sem *sem,int num);
int my_sem_sub(struct st_sem *sem,int num);

int my_sem_destroy(struct st_sem *sem);


#endif
