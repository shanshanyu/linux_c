#include <stdio.h>
#include <stdlib.h>
#include <shadow.h>

#include <unistd.h>
#include <string.h>
#include <crypt.h>
#define _XOPEN_SOURCE

//通过参数传递username
int main(int argc,char *argv[]){
  if(argc != 2){
    fprintf(stderr,"Usage:%s username\n",argv[0]);
    exit(1);
  }
  int i = 0;
  struct spwd *pw;
  char *input_pw;
  char *crypt_pw;
  pw = getspnam(argv[1]);
  if(pw == NULL){
    perror("getspnam()");
    exit(1);
  }

 // printf("pw = %s\n",pw->sp_pwdp);

  while(i < 3){
    input_pw = getpass("input your password:");
    crypt_pw = crypt(input_pw,pw->sp_pwdp);
   // printf("%s\n",crypt_pw);
    if(strcmp(pw->sp_pwdp,crypt_pw) != 0)
      printf("password incorrect,input again\n");
    else{
      printf("corret\n");
      exit(0);
    }
      
    i++;
  }
  printf("password incorrect %d times,exit!\n",i);

  exit(0);
}
