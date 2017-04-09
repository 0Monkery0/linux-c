#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc,char *argv[]){
    if(argc != 2){
        printf("输入参数个数错误!");
        return -1;
    }
    struct stat buf;
    if(stat(argv[1],&buf) == -1){
        printf("文件信息获取失败!");
        return -1;
    }
    printf("文件的设备编号为:%d\n",buf.st_dev);
    printf("文件的i-node节点标号为:%d\n",buf.st_ino);
    printf("文件的类型和存储权限为:%o\n",buf.st_mode);
    if(S_ISLNK(buf.st_mode)){
        printf("该文件为符号链接\n");
    }
    else if(S_ISREG(buf.st_mode)){
        printf("该文件为一般文件\n");
    }
    else if(S_ISDIR(buf.st_mode)){
        printf("该文件为目录文件\n");
    }
    else if(S_ISCHR(buf.st_mode)){
        printf("该文件为字符设备文件\n");
    }
    else if(S_ISBLK(buf.st_mode)){
        printf("该文件为字符设备文件\n");
    }
    else if(S_ISFIFO(buf.st_mode)){
        printf("该文件为先进先出FIFO\n");
    }
    else if(S_ISSOCK(buf.st_mode)){
        printf("该文件为socker\n");
    }
    printf("文件的硬链接数目为:%d\n",buf.st_nlink);
    printf("文件所有者的用户id为:%d\n",buf.st_uid);
    printf("文件所有者的组id为:%d\n",buf.st_gid);
    printf("文件的设备编号为(若为设备文件):%d\n",buf.st_rdev);
    printf("文件大小为:%d\n",buf.st_size);
    printf("文件的系统I/O缓冲区大小为:%d\n",buf.st_blksize);
    printf("文件的区块数目为:%d\n",buf.st_blocks);
    printf("文件最后一次被访问的时间为:%d\n",buf.st_atime);
    printf("文件最后一次被修改的时间为:%d\n",buf.st_mtime);
    printf("文件最近一次被更改的时间(属性信息):%d\n",buf.st_ctime);
    return 0;
}
