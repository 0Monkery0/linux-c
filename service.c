#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define LISTEN_NUM 16

int CreateSock();

int main(){
    int sockFd;
    sockFd = CreateSock();
    if(sockFd == -1){
        printf("Failed to start!\n");
        return -1;
    }
    return 0;
}

int CreateSock(){
    int sockFd;
    struct sockaddr_in info;

    sockFd = socket(AF_INET,SOCK_STREAM,0);
    if(sockFd == -1){
        return -1;
    }

    /*
    printf("Please input IP(Example:192.168.1.1):\n");
    scanf("%s",temp);
    while(temp[i] != '\0'){
        if(temp[i] != '.'||temp[i] < 47||temp[i] > 58){
            printf("The IP is undesirable!\n");
            return -1;
        }
        i++;
    }
    */

    info.sin_family = AF_INET;
    info.sin_port = htons(8080);
    info.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(info.sin_zero,0,sizeof(info.sin_zero));

    printf("%s\n",info.sin_addr.s_addr);

    bind(sockFd,(struct sockaddr *)&info,sizeof(struct sockaddr_in));

    listen(sockFd,LISTEN_NUM);

    return sockFd;
}
