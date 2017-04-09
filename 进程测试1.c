#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    pid_t pid;
    int k;
    char *out;
    pid = fork();
    switch(pid){
    case 0:
        out = "哈哈";
        k = 3;
        break;
    case -1:
        printf("进程出现错误\n");
        break;
    default:
        out = "呵呵";
        k = 5;
        break;
    }
    while(k>0){
        printf("%s\n",out);
        sleep(1);
        k--;
    }
    return 0;
}
