#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int wait();

int main(){
    pthread_t new;
    unsigned short state;
    pthread_create(&new,NULL,(void *)wait,NULL);
    pthread_join(new,(void *)&state);
    printf("子线程结束!\n");
    return 0;
}

int wait(){
    for(unsigned short i = 0;i < 10;i++){
        printf("子线程正在运行!\n");
        sleep(1);
    }
    pthread_exit(0);
}
