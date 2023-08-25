#include <stdio.h>
#include <stdlib.h>


//冒泡排序
void bubber_sort(int s[],int n){
  int i,j,tmp;
  for(i = 0; i < n; i++)   //控制循环次数
    for(j = 0; j < n-i-1; j++)  //每次把最大的数放到数组的最后
      if(s[j] > s[j+1]){
        tmp=s[j];
        s[j] = s[j+1];
        s[j+1] = tmp;
      } 
}

int main(){
  int a[] = {5,4,3,2,1,6,7,8,9,10};
  int i;

  bubber_sort(a,sizeof(a)/sizeof(a[0]));

  for(i = 0; i < sizeof(a)/sizeof(a[0]); i++)
    printf("%d\n",a[i]);


  exit(0);
}
