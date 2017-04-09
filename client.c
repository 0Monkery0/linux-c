#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int CreateSock();
int Connect(int sockFd);

int main(){
    int sockFd;
    sockFd = CreateSock();
    if(sockFd == -1){
        printf("Fail to start!\n");
        return -1;
    }
    Connect(sockFd);
    return 0;
}

int CreateSock(){
    int sockFd;

    sockFd = socket(AF_INET,SOCK_STREAM,0);
    if(sockFd == -1){
        return -1;
    }

    return sockFd;
}

int Connect(int sockFd){
    const char *buffer = "192.168.30.149";
    struct sockaddr_in info;

    memset(&info.sin_addr,0,sizeof(struct sockaddr_in));
    info.sin_family = AF_INET;
    info.sin_port = htons(8080);
    inet_aton(buffer,&info.sin_addr);
    memset(info.sin_zero,0,sizeof(info.sin_zero));

    connect(sockFd,(struct sockaddr *)&info,sizeof(struct sockaddr));
    return 0;
}
