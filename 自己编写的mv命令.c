#include <stdio.h>

int main(int argc,int *argv[]){
    if(argc != 3){
        printf("输入参数个数错误\n");
        return -1;
    }
    if(rename(argv[1],argv[2]) == -1){
        printf("修改文件名失败\n");
        return -1;
    }
    return 0;
}
