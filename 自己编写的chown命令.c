#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc,char *argv[]){
    int i,uid,gid;
    char *filename;
    if(argc == 4){
        int n;
        filename = argv[1];
        n = strlen(argv[2]);
        for(i = 0;i < n;i++){
            if(argv[2][i] < 47||argv[2][i] > 58){
                printf("输入参数错误!\n");
                return -1;
            }
        }
        n = strlen(argv[3]);
        for(i = 0;i <n ;i++){
            if(argv[3][i] < 47||argv[3][i] > 58){
                printf("输入参数错误!\n");
                return -1;
            }
        }
        uid = atoi(argv[2]);
        gid = atoi(argv[3]);
        if(chown(filename,uid,gid) == -1){
            printf("更改文件属性失败!\n");
            return -1;
        }
    }
    else if(argc == 3)
    {
        int len;
        FILE *fp;
        char temp,temp_str[50],owner[20],group[20];
        filename = argv[2];
        i = 0;
        while(1){
            temp = argv[1][i];
            i++;
            if(temp == ':')
                break;
            else
                owner[i-1] = temp;
        };
        owner[i-1]='\0';
        i = 0;
        len = strlen(owner);
        while(1){
            temp = argv[1][i+len+1];
            i++;
            if(temp == '\0')
                break;
            else
                group[i-1] = temp;
        };
        group[i-1] = '\0';
        fp = fopen("/etc/passwd","r");
        while(1){
            i = 0;
            while(1){
                temp = fgetc(fp);
                if(temp != ':'&&temp != '\n'){
                    temp_str[i] = temp;
                    i++;
                }
                else{
                    temp_str[i] = '\0';
                    break;
                }
            };
            if(strcmp(temp_str,owner) == 0)
                break;
        };
        temp = fgetc(fp);
        temp = fgetc(fp);
        i = 0;
        while(1){
            temp = fgetc(fp);
            if(temp != ':'){
                temp_str[i] = temp;
                i++;
            }
            else
                break;
        };
        temp_str[i] = '\0';
        uid = atoi(temp_str);
        fp = fopen("/etc/passwd","r");
        while(1){
            i = 0;
            while(1){
                temp = fgetc(fp);
                if(temp != ':'&&temp != '\n'){
                    temp_str[i] = temp;
                    i++;
                }
                else{
                    temp_str[i] = '\0';
                    break;
                }
            };
            if(strcmp(temp_str,group) == 0)
                break;
        };
        for(i = 0;i < 2;){
            temp = fgetc(fp);
            if(temp == ':')
                i++;
        }
        i = 0;
        while(1){
            temp = fgetc(fp);
            if(temp != ':'){
                temp_str[i] = temp;
                i++;
            }
            else
                break;
        };
        temp_str[i] = '\0';
        gid = atoi(temp_str);
        if(chown(filename,uid,gid) == -1){
            printf("更改文件属性失败!\n");
            return -1;
        }
    }
    else
        printf("输入参数数量错误!\n");
    return 0;
}
