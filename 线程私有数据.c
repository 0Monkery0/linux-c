#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_key_t key;

void *thread1();
void *thread2();
int main(){
    pthread_t thd1;
    printf("Thread 0 is running!\n");
    pthread_key_create(&key,NULL);
    pthread_create(&thd1,NULL,thread1,NULL);
    sleep(5);
    printf("Down!\n");
    return 0;
}

void *thread1(){
    int text = 5;
    pthread_t thd2;
    printf("Thread 1 is running!\n");
    pthread_setspecific(key,(void *)text);
    text--;
    printf("The 1 text number is %d!\n",pthread_getspecific(key));
    pthread_create(&thd2,NULL,thread2,NULL);
    sleep(2);
    pthread_exit(0);
}

void *thread2(){
    int text = 5;
    printf("Thread 2 is running!\n");
    pthread_setspecific(key,(void *)text);
    printf("The 3 text number is %d!\n",pthread_getspecific(key));
    pthread_exit(0);
}
