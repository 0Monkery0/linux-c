#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int fd;
    fd = creat("text.c",511);
    close(fd);
    return 0;
}
