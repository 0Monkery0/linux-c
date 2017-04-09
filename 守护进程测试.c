#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<sys/param.h>
#include<sys/stat.h>
#include<time.h>
#include<syslog.h>

int creat_deamon();

int main(){
    time_t now;
    creat_deamon();
    syslog(LOG_USER|LOG_INFO,"测试守护进程!\n");
    while(1){
        sleep(10);
        time(&now);
        syslog(LOG_USER|LOG_INFO,"当前系统时间为:\t%s\t\n",ctime(&now));
    }
    return 0;
}

int creat_deamon(){
    pid_t pid;
    int i;

    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGHUP,SIG_IGN);

    pid = fork();
    if(pid != 0){
         exit(0);
    }

    setsid();

    pid = fork();
    if(pid > 0){
        exit(0);
    }

    for(i = 0;i<NOFILE;close(i++));

    chdir("/");

    umask(0);

    signal(SIGCHLD,SIG_IGN);

    return 0;
}
