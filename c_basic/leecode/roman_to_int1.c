#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int ch_to_int(char *s,int *index){
    int ch;
    ch = s[*index];
    int i = 0;
    switch(ch){
        case 'I':
            i = 1;
            break;
        case 'V':
            i = 5;
            if(s[*index-1] == 'I'){
                i -= 1;
                (*index)--;  //指向数组的指针可以这样操作
            }
            break;
        case 'X':
            i = 10;
            if(s[*index-1] == 'I'){
                i -= 1;
                (*index)--;
            }
            break;
        case 'L':
            i = 50;
            if(s[*index-1] == 'X'){
                i -= 10;
                (*index)--;
            }
            break;
        case 'C':
            i = 100;
            if(s[*index-1] == 'X'){
                i -= 10;
                (*index)--;  //指向数组的指针可以这样操作
            }
            break;
        case 'D':
            i = 500;
            if(s[*index-1] == 'C'){
                i -= 100;
                (*index)--;
            }
            break;
        case 'M':
            i = 1000;
            if(*index > 0 && s[*index-1] == 'C'){  //前面要求还有一个字符
                i -= 100;
                (*index)--;  //指向数组的指针可以这样操作
            }
            break;
        default:
            printf("invalid roman number\n");
            i = -1;
    }
    (*index)--;
    return i;
}


int romanToInt(char * s){
    int index; //字符串索引位置
    int sum = 0;
    int ret;
    index = strlen(s)-1;
    while(index >= 0){
        ret = ch_to_int(s,&index);
        if(ret < 0)
            return -1;
        sum += ret;
    }
    
    
    return sum;
}


int main(){
  char s[] = "LVIII";
  printf("%d",romanToInt(s));
  exit(0);
}
