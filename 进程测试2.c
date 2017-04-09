#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int num = 0;

void main(){
    pid_t pid;
    int tea = 20,i;
    pid = vfork();
    switch(pid){
    case 0:
        i = 5;
        while(i>0){
            printf("1\n");
            num++;
            tea++;
            i--;
        }
        exit(0);
        break;
    case -1:
        printf("进程错误\n");
        break;
    default:
        i = 5;
        while(i>0){
            printf("2\n");
            num++;
            tea++;
            i--;
        }
    }
    printf("%d %d\n",num,tea);
    exit(0);
}
