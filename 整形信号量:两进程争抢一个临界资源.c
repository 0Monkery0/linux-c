#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int wait(int n);
int signal(int n);
unsigned short test(int n);

int main(int argc,char *argv[]){
    int mutex = 1;
    pid_t pid;
    pid = fork();
    test(mutex);
    return 0;
}

//整形信号量P操作
int wait(int n){
    while(n <= 0);
    n--;
    return n;
}

//整形信号量V操作
int signal(int n){
    n++;
    printf("PID%d down\n",getpid());
    return n;
}

unsigned short test(int n){
    while(1){
        n = wait(n);
        sleep(3);//模拟资源占用时间
        n = signal(n);
    };
    return 0;
}
