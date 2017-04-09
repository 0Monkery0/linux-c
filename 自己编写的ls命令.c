#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ioctl.h>
#include<dirent.h>
#include<termios.h>
#include<signal.h>
#include<unistd.h>
#include<pwd.h>
#include<grp.h>

struct file{
    int len[10];
    unsigned short go;
    int realfilename_len;
    char filename[NAME_MAX+1];
    char filepath[PATH_MAX+1];
    struct file *previous;
    struct file *next;
};

struct dir_info{
    char path[PATH_MAX+1];
    struct dir_info *next;
};

char *path_judge(unsigned short go,char *path);
int a_judge(char *path,unsigned short pm_a,struct file *phead);
int u_t_r_judge(unsigned short pm_u,unsigned short pm_t,unsigned short pm_r,unsigned short pm_l,struct file *phead);
int sort_name(struct file *phead);
int sort_atime(struct file *phead);
int sort_mtime(struct file *phead);
int sort_reverse(struct file *phead);
int maxinode_len(struct file *phead);
int maxlink_len(struct file *phead);
int maxuid_len(struct file *phead);
int maxgid_len(struct file *phead);
int maxsize_len(struct file *phead);
int display_l_i(int len_inode,int len_link,int len_uid,int len_gid,int len_size,struct file *phead,unsigned short pm_i);
int display(unsigned short pm_i,unsigned short pm_l,struct file *phead);
int realnamelen(struct file *phead,unsigned short pm_i);
int terminal_info();
int line_num(int col,struct file *phead);
int real_icount(int col,int icount,struct file *phead,unsigned short pm_i);
int display_null(int icount,struct file *phead,unsigned short pm_i);
int display_text(struct file *phead);

int main(int argc,char *argv[]){

    unsigned short i,j,go = 0,pm_i = 0,pm_r = 0,pm_t = 0,pm_a = 0,pm_l = 0,pm_R = 0,pm_u = 0;
    char *path,temp[50];
    struct file *phead;
    phead = (struct file *)malloc(sizeof(struct file));
    phead->next = NULL;
    phead->previous = NULL;
    phead->go = 0;

    //分析参数
    if(argc > 1){
        for(i = 0;i<argc-1;i++){
            if(argv[i+1][0] == '/'&&go != 1){
                path = argv[i+1];
                go = 1;
            }
            else if(argv[i+1][0] == '.'&&argv[i+1][1] == '/'&&go != 1){
                path = getcwd(NULL,0);
                for(j = 1;j<strlen(argv[i+1]);j++){
                    temp[j-1]=argv[i+1][j];
                }
                strcat(path,temp);
                go = 1;
            }
            else if(argv[i+1][0] == '-'){
                for(j = 0;j<strlen(argv[i+1])-1;j++){
                    if(argv[i+1][j+1] == 'i'&&pm_i != 1){
                        pm_i = 1;
                    }
                    else if(argv[i+1][j+1] == 'r'&&pm_r != 1){
                        pm_r = 1;
                    }
                    else if(argv[i+1][j+1] == 't'&&pm_t != 1){
                        pm_t = 1;
                    }
                    else if(argv[i+1][j+1] == 'a'&&pm_a != 1){
                        pm_a = 1;
                    }
                    else if(argv[i+1][j+1] == 'l'&&pm_l != 1){
                        pm_l = 1;
                    }
                    else if(argv[i+1][j+1] == 'R'&&pm_R != 1){
                        pm_R = 1;
                    }
                    else if(argv[i+1][j+1] == 'u'&&pm_u != 1){
                        pm_u = 1;
                    }
                    else{
                        printf("输入的参数未支持!(或输入参数有重复)\n");
                        return -1;
                    }
                }
            }
            else{
                path = getcwd(NULL,0);
                strcat(path,"/");
                for(j = 0;j<strlen(argv[i+1]);j++){
                    temp[j]=argv[i+1][j];
                }
                strcat(path,temp);
                if(go == 1){
                    printf("输入参数错误!\n");
                    return -1;
                }
                go = 1;
            }
        }
    }

    //对路径进行检查
    path = path_judge(go,path);
    if(path == NULL){
        return -1;
    }
    chdir(path);

    if(pm_R == 0){
        if(a_judge(path,pm_a,phead) == 0){
            u_t_r_judge(pm_u,pm_t,pm_r,pm_l,phead);
            display(pm_i,pm_l,phead);///
        }
    }
    else if(pm_R == 1){///////////////////////////////////////////////////////////////////////////
        DIR *dir;
        struct dirent *info;
        struct stat buf;

        char path_temp[PATH_MAX];
        struct file *ptemp;

        struct dir_info *pheaddir,*pnewdir,*ptempdir,*pfreedir;
        pheaddir = (struct dir_info *)malloc(sizeof(struct dir_info));
        pheaddir->next = NULL;
        strcpy(pheaddir->path,path);
        ptempdir = pheaddir;

        do{
            strcpy(path,pheaddir->path);
            chdir(path);
            if(a_judge(path,pm_a,phead) == 0){
                u_t_r_judge(pm_u,pm_t,pm_r,pm_l,phead);
                printf("%s\n",path);
                display(pm_i,pm_l,phead);///
                printf("\n");
                ptemp = phead->next;
                while(ptemp->go != 0){
                    lstat(ptemp->filename,&buf);
                    if(S_ISDIR(buf.st_mode)){
                        pnewdir = (struct dir_info *)malloc(sizeof(struct dir_info));
                        pnewdir->next = NULL;
                        ptempdir->next = pnewdir;
                        ptempdir = ptempdir->next;
                        strcpy(path,"\0");
                        path = getcwd(NULL,0);
                        strcpy(path_temp,"\0");
                        strcat(path_temp,"/");
                        strcat(path_temp,ptemp->filename);
                        strcat(path,path_temp);
                        strcpy(ptempdir->path,path);
                    }
                    ptemp = ptemp->next;
                }
            }
            pfreedir = pheaddir;
            pheaddir = pheaddir->next;
            free(pfreedir);
            pfreedir = NULL;
        }
        while(pheaddir != NULL);
    }////////////////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}

char *path_judge(unsigned short go,char *path){
    if(go != 1){
        path = getcwd(NULL,0);
        return path;
    }
    if(opendir(path) == NULL){
        printf("输入路径错误!\n");
        return NULL;
    }
    return path;
}

int a_judge(char *path,unsigned short pm_a,struct file *phead){
    DIR *dir;
    struct dirent *info;
    struct file *ptemp,*pnew;

    if(phead->next != NULL){
        ptemp = phead->next;
        pnew = ptemp->next;
        while(pnew->go != 0){
            free(ptemp);
            pnew = pnew->next;
            ptemp = pnew->previous;
        }
        free(ptemp);
        ptemp = NULL;
    }

    phead->next = NULL;
    phead->previous = NULL;
    ptemp = phead;
    dir = opendir(path);
    if(pm_a == 0){
        while((info = readdir(dir)) != NULL){
            if(info->d_name[0] != '.'){
                pnew = (struct file *)malloc(sizeof(struct file));
                pnew->go = 1;
                pnew->next = phead;
                pnew->previous = ptemp;
                ptemp->next = pnew;
                phead->previous = pnew;
                strcpy(pnew->filename,info->d_name);
                ptemp = pnew;
            }
        }
    }
    else if(pm_a == 1){
        while((info = readdir(dir)) != NULL){
            pnew = (struct file *)malloc(sizeof(struct file));
            pnew->go = 1;
            pnew->next = phead;
            pnew->previous = ptemp;
            ptemp->next = pnew;
            phead->previous = pnew;
            strcpy(pnew->filename,info->d_name);
            ptemp = pnew;
        }
    }
    closedir(dir);
    ptemp = NULL;
    pnew = NULL;
    if(phead->next == NULL){
        printf("%s\n",path);
        printf("该目录下没有找到文件!\n\n");
        return -1;
    }
    return 0;
}

int u_t_r_judge(unsigned short pm_u,unsigned short pm_t,unsigned short pm_r,unsigned short pm_l,struct file *phead){
    struct file *ptemp,*ptemp2;

    ptemp = phead->next;
    if(ptemp == NULL){
        return -1;
    }
    ptemp2 = ptemp->next;
    if(ptemp2 == NULL){
        return -1;
    }

    if(pm_u == 1&&pm_l == 1){
        if(pm_t == 1){
            sort_atime(phead);
        }
        else{
            sort_name(phead);
        }
    }
    else if(pm_u == 1&&pm_l == 0){
        sort_atime(phead);
    }
    else if(pm_u == 0&&pm_t == 1){
        sort_mtime(phead);
    }
    else{
        sort_name(phead);
    }

    if(pm_r == 1){
        sort_reverse(phead);
    }
    return 0;
}

int sort_name(struct file *phead){
    unsigned short go;
    struct file *ptemp,*ptemp2;
    while(1){
        ptemp = phead->next;
        ptemp2 = ptemp->next;
        go = 0;
        while(ptemp2->go != 0){
            if(strcmp(ptemp->filename,ptemp2->filename)>0){
                strcpy(phead->filename,ptemp->filename);
                strcpy(ptemp->filename,ptemp2->filename);
                strcpy(ptemp2->filename,phead->filename);
                go = 1;
            }
            ptemp = ptemp->next;
            ptemp2 = ptemp2->next;
        }
        if(go == 0){
            break;
        }
    }
    return 0;
}

int sort_atime(struct file *phead){
    struct stat buf;
    long temp_t,temp_t2;
    unsigned short go;
    struct file *ptemp,*ptemp2;
    while(1){
        ptemp = phead->next;
        ptemp2 = ptemp->next;
        go = 0;
        while(ptemp2->go != 0){
            lstat(ptemp->filename,&buf);
            temp_t = buf.st_atime;
            lstat(ptemp2->filename,&buf);
            temp_t2 = buf.st_atime;
            if(temp_t<temp_t2){
                strcpy(phead->filename,ptemp->filename);
                strcpy(ptemp->filename,ptemp2->filename);
                strcpy(ptemp2->filename,phead->filename);
            }
            ptemp = ptemp->next;
            ptemp2 = ptemp2->next;
        }
        if(go == 0){
            break;
        }
    }
    return 0;
}

int sort_mtime(struct file *phead){
    struct stat buf;
    long temp_t,temp_t2;
    unsigned short go;
    struct file *ptemp,*ptemp2;
    while(1){
        ptemp = phead->next;
        ptemp2 = ptemp->next;
        go = 0;
        while(ptemp2->go != 0){
            lstat(ptemp->filename,&buf);
            temp_t = buf.st_mtime;
            lstat(ptemp2->filename,&buf);
            temp_t2 = buf.st_mtime;
            if(temp_t<temp_t2){
                strcpy(phead->filename,ptemp->filename);
                strcpy(ptemp->filename,ptemp2->filename);
                strcpy(ptemp2->filename,phead->filename);
            }
            ptemp = ptemp->next;
            ptemp2 = ptemp2->next;
        }
        if(go == 0){
            break;
        }
    }
    return 0;
}

int sort_reverse(struct file *phead){
    unsigned short i;
    int icount = 0;
    struct file *forward,*reverse;
    forward = phead->next;
    while(forward->go != 0){
        icount++;
        forward = forward->next;
    }
    forward = phead->next;
    reverse = phead->previous;
    for(i = 0;i<icount/2;i++){
        strcpy(phead->filename,forward->filename);
        strcpy(forward->filename,reverse->filename);
        strcpy(reverse->filename,phead->filename);
        forward = forward->next;
        reverse = reverse->previous;
    }
    return 0;
}

int display(unsigned short pm_i,unsigned short pm_l,struct file *phead){
    int len_inode,len_link,len_uid,len_gid,len_size,col,icount;
    len_inode = maxinode_len(phead);
    len_link = maxlink_len(phead);
    len_uid = maxuid_len(phead);
    len_gid = maxgid_len(phead);
    len_size = maxsize_len(phead);
    if(pm_l == 1){
        display_l_i(len_inode,len_link,len_uid,len_gid,len_size,phead,pm_i);
    }
    else{
        realnamelen(phead,pm_i);
        col = terminal_info();
        icount = line_num(col,phead);
        icount = real_icount(col,icount,phead,pm_i);
        display_null(icount,phead,pm_i);
    }
    return 0;
}

int maxinode_len(struct file *phead){
    char temp[10];
    int len = 0;
    struct stat buf;
    struct file *ptemp;
    ptemp = phead->next;
    while(ptemp->go != 0){
        lstat(ptemp->filename,&buf);
        sprintf(temp,"%d\0",buf.st_ino);
        if(strlen(temp)>len){
            len = strlen(temp);
        }
        ptemp = ptemp->next;
    }
    return len;
}

int maxlink_len(struct file *phead){
    char temp[5];
    int len = 0;
    struct stat buf;
    struct file *ptemp;
    ptemp = phead->next;
    while(ptemp->go != 0){
        lstat(ptemp->filename,&buf);
        sprintf(temp,"%d\0",buf.st_nlink);
        if(strlen(temp)>len){
            len = strlen(temp);
        }
        ptemp = ptemp->next;
    }
    return len;
}

int maxuid_len(struct file *phead){
    struct passwd *info;
    int len = 0;
    struct stat buf;
    struct file *ptemp;
    ptemp = phead->next;
    while(ptemp->go != 0){
        lstat(ptemp->filename,&buf);
        info = getpwuid(buf.st_uid);
        if(strlen(info->pw_name)>len){
            len = strlen(info->pw_name);
        }
        ptemp = ptemp->next;
    }
    return len;
}

int maxgid_len(struct file *phead){
    struct group *info;
    int len = 0;
    struct stat buf;
    struct file *ptemp;
    ptemp = phead->next;
    while(ptemp->go != 0){
        lstat(ptemp->filename,&buf);
        info = getgrgid(buf.st_gid);
        if(strlen(info->gr_name)>len){
            len = strlen(info->gr_name);
        }
        ptemp = ptemp->next;
    }
    return len;
}

int maxsize_len(struct file *phead){
    char temp[10];
    int len = 0;
    struct stat buf;
    struct file *ptemp;
    ptemp = phead->next;
    while(ptemp->go != 0){
        lstat(ptemp->filename,&buf);
        sprintf(temp,"%d\0",buf.st_size);
        if(strlen(temp)>len){
            len = strlen(temp);
        }
        ptemp = ptemp->next;
    }
    return len;
}

int display_l_i(int len_inode,int len_link,int len_uid,int len_gid,int len_size,struct file *phead,unsigned short pm_i){
    char temp_str[50];
    unsigned short i;
    int temp;
    struct passwd *usr;
    struct group *grp;
    struct stat buf;
    struct file *ptemp;

    ptemp = phead->next;
    temp = 0;
    while(ptemp->go != 0){
        lstat(ptemp->filename,&buf);
        temp += buf.st_blocks;
        ptemp = ptemp->next;
    }
    temp = temp/2;
    printf("总用量%d\n",temp);

    ptemp = phead->next;
    while(ptemp->go != 0){
        lstat(ptemp->filename,&buf);

        if(pm_i == 1){
            sprintf(temp_str,"%d\0",buf.st_ino);
            temp = strlen(temp_str);
            for(i = 0;i<len_inode-temp;i++){
                printf(" ");
            }
            printf("%s ",temp_str);
        }

        printf("-");//没有获取第一位权限位
        temp = buf.st_mode&S_IRUSR;
        if(temp == S_IRUSR){
            printf("r");
        }
        else{
            printf("-");
        }
        temp = buf.st_mode&S_IWUSR;
        if(temp == S_IWUSR){
            printf("w");
        }
        else{
            printf("-");
        }
        temp = buf.st_mode&S_IXUSR;
        if(temp == S_IXUSR){
            printf("x");
        }
        else{
            printf("-");
        }
        temp = buf.st_mode&S_IRGRP;
        if(temp == S_IRGRP){
            printf("r");
        }
        else{
            printf("-");
        }
        temp = buf.st_mode&S_IWGRP;
        if(temp == S_IWGRP){
            printf("w");
        }
        else{
            printf("-");
        }
        temp = buf.st_mode&S_IXGRP;
        if(temp == S_IXGRP){
            printf("x");
        }
        else{
            printf("-");
        }
        temp = buf.st_mode&S_IROTH;
        if(temp == S_IROTH){
            printf("r");
        }
        else{
            printf("-");
        }
        temp = buf.st_mode&S_IWOTH;
        if(temp == S_IWOTH){
            printf("w");
        }
        else{
            printf("-");
        }
        temp = buf.st_mode&S_IXOTH;
        if(temp == S_IXOTH){
            printf("x");
        }
        else{
            printf("-");
        }
        printf(" ");

        sprintf(temp_str,"%d\0",buf.st_nlink);
        temp = strlen(temp_str);
        for(i = 0;i<len_link-temp;i++){
            printf(" ");
        }
        printf("%s ",temp_str);

        usr = getpwuid(buf.st_uid);
        temp = strlen(usr->pw_name);
        for(i = 0;i<len_uid-temp;i++){
            printf(" ");
        }
        printf("%s ",usr->pw_name);

        grp = getgrgid(buf.st_gid);
        temp = strlen(grp->gr_name);
        for(i = 0;i<len_gid-temp;i++){
            printf(" ");
        }
        printf("%s ",grp->gr_name);

        sprintf(temp_str,"%d\0",buf.st_size);
        temp = strlen(temp_str);
        for(i = 0;i<len_size-temp;i++){
            printf(" ");
        }
        printf("%s ",temp_str);

        strcpy(temp_str,ctime(&buf.st_mtime));
        temp = strlen(temp_str)-1;
        for(i = 0;i<temp;i++){
            printf("%c",temp_str[i]);
        }
        printf(" ");

        if(S_ISDIR(buf.st_mode)){
            printf("\033[1;34;40m%s\033[0m",ptemp->filename);
        }
        else{
            printf("%s",ptemp->filename);
        }

        printf("\n");
        ptemp = ptemp->next;
    }
    return 0;
}

//计算每个文件名的显示宽度
int realnamelen(struct file *phead,unsigned short pm_i){
    int namelen_temp,i,j;
    struct file *ptemp;
    ptemp = phead->next;
    while(ptemp->go != 0){
        namelen_temp = strlen(ptemp->filename);
        if(pm_i == 1){
            namelen_temp = namelen_temp+7;
        }
        i = 0;
        j = 0;
        while(1){
            if(ptemp->filename[i] == '\0'){
                break;
            }
            if(ptemp->filename[i]>47&&ptemp->filename[i]<58||ptemp->filename[i]>64&&ptemp->filename[i]<91||ptemp->filename[i]>96&&ptemp->filename[i]<123||ptemp->filename[i]=='_'||ptemp->filename[i]=='.'||ptemp->filename[i]=='-'){
                ;
            }
            else{
                j++;
                if(j == 3){
                    j = 0;
                    namelen_temp--;
                }
            }
            i++;
        }
        ptemp->realfilename_len = namelen_temp;
        ptemp = ptemp->next;
    }
    return 0;
}

//获取当前终端信息
int terminal_info(){
    struct winsize info;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&info);
    return info.ws_col;
}

//计算每行可显示的文件数
int line_num(int col,struct file *phead){
    int pathname_sum,icount = 10,icount_temp;
    struct file *ptemp;
    ptemp = phead->next;
    while(ptemp->go != 0){
        icount_temp = 0;
        pathname_sum = 0;
        while(pathname_sum<col){
            if(ptemp->go == 0){
                break;
            }
            pathname_sum += ptemp->realfilename_len;
            icount_temp++;
            if(icount_temp == 10){
                break;
            }
            if(pathname_sum,col){
                ptemp = ptemp->next;
            }
        }
        if(icount>icount_temp){
            icount = icount_temp;
        }
    }
    return icount;
}

//计算每一列显示信息的最大长度并重新计算每一行显示的文件数
int real_icount(int col,int icount,struct file *phead,unsigned short pm_i){
    unsigned short i,go,icount_temp;
    struct file *ptemp;
    int pathname_sum;

    while(1){
        pathname_sum = 0;
        while(icount != icount_temp){
            go = 0;
            ptemp = phead->next;
            for(i = 0;i<icount_temp;i++){
                ptemp = ptemp->next;
            }
            phead->len[icount_temp] = ptemp->realfilename_len;
            while(1){
                for(i = 0;i<icount;i++){
                    ptemp = ptemp->next;
                    if(ptemp->go == 0){
                        go = 1;
                        break;
                    }
                }
                if(go == 1){
                    break;
                }
                if(ptemp->realfilename_len>phead->len[icount_temp]){
                    phead->len[icount_temp] = ptemp->realfilename_len;
                }
            }
            pathname_sum += phead->len[icount_temp];
            icount_temp++;
        }
        if(pathname_sum>col){
            icount--;
            icount_temp = 0;
            i = 0;
        }
        else{
            break;
        }
    }

    return icount;
}

int display_null(int icount,struct file *phead,unsigned short pm_i){
    unsigned short i;
    int icount_temp = 0;
    struct file *ptemp;
    struct stat buf;
    ptemp = phead->next;
    while(ptemp->go != 0){
        if(pm_i == 1){
            stat(ptemp->filename,&buf);
            printf("%7d ",buf.st_ino);
        }
        if(S_ISDIR(buf.st_mode)){
            printf("\033[1;34;40m%s\033[0m",ptemp->filename);
        }
        else{
            printf("%s",ptemp->filename);
        }
        for(i = 0;i<phead->len[icount_temp%icount]-ptemp->realfilename_len+2;i++){
            printf(" ");
        }
        icount_temp++;
        ptemp = ptemp->next;
        if(icount_temp == icount){
            printf("\n");
            icount_temp = 0;
        }
    }
    printf("\n");
    return 0;
}

int display_text(struct file *phead){
    struct file *ptemp;
    ptemp = phead->next;
    while(ptemp->go != 0){
        printf("%s\n",ptemp->filename);
        ptemp = ptemp->next;
    }
    return 0;
}
