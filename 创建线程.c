#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void *thread(void *arg);

int main(){
    pthread_t now;

    printf("父线程id为:%u\n",pthread_self());
    if(pthread_create(&now,NULL,(void *)thread,NULL) != 0){
        printf("线程创建失败!\n");
        exit(1);
    }
    sleep(1);
    return 0;
}

void *thread(void *arg){
    pthread_t new;
    new = pthread_self();
    printf("子线程id为:%u\n",new);
    return NULL;
}
