#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char *argv[]){
    int modinput;
    int mod_owner;
    int mod_group;
    int mod_other;
    char *filename;
    if(argc != 3){
        printf("输入参数个数错误!\n");
        return (-1);
    }
    if(strlen(argv[1]) != 3){
        printf("输入的权限参数错误!\n");
        return (-1);
    }
    modinput = atoi(argv[1]);
    mod_other = modinput%10;
    if(mod_other<0||mod_other>7){
        printf("输入的权限参数错误!\n");
        return (-1);
    }
    mod_group = (modinput%100-mod_other)/10;
    if(mod_group<0||mod_group>7){
        printf("输入的权限参数错误!\n");
        return (-1);
    }
    mod_owner = (modinput-mod_other-mod_group*10)/100;
    if(mod_owner<0||mod_owner>7){
        printf("输入的权限参数错误!\n");
        return (-1);
    }
    filename = argv[2];
    modinput = mod_other+mod_group*8+mod_owner*8*8;
    if(chmod(filename,modinput) == -1){
        printf("权限更改失败");
        return (-1);
    }
    return 0;
}
