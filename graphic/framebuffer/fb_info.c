#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(){
  int fd;
  struct fb_var_screeninfo vinfo;

  fd = open("/dev/fb0",O_RDONLY);
  if(fd < 0){
    perror("open()");
    exit(1);
  }

  if(ioctl(fd,FBIOGET_VSCREENINFO,&vinfo) < 0){
    perror("ioctl()");
    exit(1);
  }

  printf("实际屏幕分辨率：x: %d,y: %d,色深bbp：%d\n",vinfo.xres,vinfo.yres,vinfo.bits_per_pixel);
  printf("虚拟屏幕分辨率：x: %d,y: %d\n",vinfo.xres_virtual,vinfo.yres_virtual);
  printf("偏移 x: %d,y:%d\n",vinfo.xoffset,vinfo.yoffset);
 

  exit(0);
}
