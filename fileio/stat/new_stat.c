#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>




int main(int argc,const char *argv[]){
  if(argc != 2){
    fprintf(stderr,"Usage:%s filename\n",argv[0]);
    exit(1);
  }

  //取得文件信息
  struct stat st;
  if(stat(argv[1],&st) == -1){
    perror("stat()");
    exit(1);
  }

  printf("file mode:");
  //file mode
  switch(st.st_mode & S_IFMT){
    case S_IFSOCK: putchar('s'); break;
    case S_IFLNK: putchar('l'); break;
    case S_IFREG: putchar('-'); break;
    case S_IFBLK: putchar('b'); break;
    case S_IFDIR: putchar('d'); break;
    case S_IFCHR: putchar('c'); break;
    case S_IFIFO: putchar('f');break;
    default: putchar('?');break;
  }
  
  //usr permission
   (st.st_mode & S_IRUSR) ? putchar('r') : putchar('-');
   (st.st_mode & S_IWUSR) ? putchar('w') : putchar('-');
   (st.st_mode & S_IXUSR) ? putchar('x') : putchar('-');
    
  //group perssion
  (st.st_mode & S_IRGRP) == S_IRGRP ? putchar('r') : putchar('-');
  (st.st_mode & S_IWGRP) == S_IWGRP ? putchar('w') : putchar('-');
  (st.st_mode & S_IXGRP) == S_IXGRP ? putchar('x') : putchar('-');

  //other permission
  (st.st_mode & S_IROTH) == S_IROTH ? putchar('r') : putchar('-');
  (st.st_mode & S_IWOTH) == S_IWOTH ? putchar('w') : putchar('-');
  (st.st_mode & S_IXOTH) == S_IXOTH ? putchar('x') : putchar('-');
  printf("\n");
 
  (st.st_mode & S_ISUID) ? printf("set uid\n") : printf("not set uid\n");
  (st.st_mode & S_ISGID) ? printf("set gid\n") : printf("not set gid\n");
  (st.st_mode & S_ISVTX) ? printf("sticky set\n") : printf("sticky not set\n");
  exit(0);
}
