#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
    pid_t pid;
    pid = fork();
    if(pid == 0){
        printf(" you!");
    }
    else if(pid == -1){
        printf("子进程创建失败!");
    }
    else{
        printf("I love");
    }
    return 0;
}
