#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<dirent.h>
#include<pwd.h>
#include<readline/readline.h>
#include<readline/history.h>

int start_home();
int print_userinfo();
int get_inputinfo(char *buffer);
int build_history(char *buffer);
int quit(char *buffer);
int analyze_cmd(char *buffer,int parameter_num,char parameter[100][256]);
int clear(char parameter[100][256]);
int go_cmd(char *buffer,int parameter_num,char parameter[100][256]);
unsigned short analyze_cmd_twice(char *cmd[25]);
int cmd_inredirection(unsigned short num,char *cmd[25]);
int cmd_outredirection(unsigned short num,char *cmd[25]);
int cmd_history(char *cmd[10]);
int cmd_help(char *cmd[1]);
int cmd_hiscln(char *cmd[1]);
int cmd_cd(char *cmd[2]);
int check_cmd(char *temp);

int main(int argc,char *argv[]){
	if(start_home() == -1){
		return -1;
	}
    int parameter_num;
    char parameter[100][256];
    char *buffer = NULL;
	signal(SIGINT,SIG_IGN);

    buffer = (char *)malloc(256);
    if(buffer == NULL){
        printf("建立缓冲区失败!\n");
        free(buffer);
        buffer = NULL;
        exit(-1);
    }

    while(1){
        memset(buffer,0,256);
        parameter_num = 0;
        clear(parameter);
        print_userinfo();
        if(get_inputinfo(buffer) == -1){
            continue;
        }
        quit(buffer);
        parameter_num = analyze_cmd(buffer,parameter_num,parameter);
        go_cmd(buffer,parameter_num,parameter);
    }
    return 0;
}

int start_home(){
	char home[50];
    struct passwd *usrname;
	usrname = getpwuid(getuid());
	strcpy(home,"/home/");
	strcat(home,usrname->pw_name);
	if(chdir(home) == -1){
		printf("初始化HOME目录失败!\n");
		return -1;
	}
	return 0;
}

int print_userinfo(){
    unsigned short go = 0,i;
    int icount = 0;
    char *temp_p,*temp_p2;
    char temp[50];
    struct passwd *usrname;
    usrname = getpwuid(getuid());
    printf("\033[1;31;40m%s@\033[0m",usrname->pw_name);
    gethostname(temp,sizeof(temp));
    printf("\033[1;31;40m%s:\033[0m",temp);
	memset(temp,0,50);
    temp_p2 = getcwd(NULL,0);
    if(getuid() == 0){
        printf("%s#",temp_p2);
    }
    else{
        temp_p = getenv("HOME");
        while(temp_p[icount] != '\0'){
            icount++;
        }
        for(i = 0;i < icount;i++){
            if(temp_p[i] != temp_p2[i]){
                go = 1;
            }
        }
        if(go != 1){
            while(temp_p2[i] != '\0'){
                temp[i-icount] = temp_p2[i];
                i++;
            }
            temp[i-icount+1] = '\0';
            printf("\033[1;35;40m~%s$\033[0m",temp);
        }
        else{
            printf("\033[1;35;40m%s$\033[0m",temp_p2);
        }
    }
    fflush(stdout);
    return 0;
}

int get_inputinfo(char *buffer){
    unsigned short i = 0;
    char *cmd;
    cmd = readline(" ");
    while(cmd[i] == ' '){
        i++;
    }
    if(cmd[i] == '\0'){
        return -1;
    }
    strcat(buffer,cmd);
    strcat(buffer,"\n");
    add_history(cmd);
    build_history(buffer);
    i = 0;
    while(buffer[i] != '\0'){
        i++;
    }
    if(i >= 256){
        printf("输入参数过长,无法解析!\n");
        return -1;
    }
    return 0;
}

int build_history(char *buffer){
    FILE *fp;
    static unsigned short number = 1;
    char home[50];
    struct passwd *usrname;
    if(getuid() == 0){
        strcpy(home,"/root");
    }
	else{
		usrname = getpwuid(getuid());
		strcpy(home,"/home/");
		strcat(home,usrname->pw_name);
	}
	strcat(home,"/history.txt");
    fp = fopen(home,"a+");
    fprintf(fp," %4lu ",number);
    fputs(buffer,fp);
    fclose(fp);
    number++;
    return 0;
}

int quit(char *buffer){
    char temp[10];
    unsigned short i = 0,go = 0,num = 0;

    while(buffer[i] == ' '){
        i++;
        num++;
    }

    for(i = 0;i < 4;i++){
        temp[i] = buffer[i+num];
    }
    temp[i] = '\0';
    while(buffer[i+num] != '\n'){
        if(buffer[i+num] != ' '){
            go = 1;
            break;
        }
        i++;
    }
    if(strcmp(temp,"exit") == 0&&go != 1){
        free(buffer);
        buffer = NULL;
        exit(0);
    }

    go = 0;
    for(i = 0;i < 6;i++){
        temp[i] = buffer[i+num];
    }
    temp[i] = '\0';
    while(buffer[i+num] != '\n'){
        if(buffer[i+num] != ' '){
            go = 1;
            break;
        }
        i++;
    }
    if(strcmp(temp,"logout") == 0&&go != 1){
        free(buffer);
        buffer = NULL;
        exit(0);
    }
    return 0;
}

int clear(char parameter[100][256]){
    unsigned short i,j;
    for(i = 0;i < 100;i++){
        for(j = 0;j < 256;j++){
            parameter[i][j] = '\0';
        }
    }
    return 0;
}

int analyze_cmd(char *buffer,int parameter_num,char parameter[100][256]){
    char *head = buffer,*end = buffer;
    unsigned short num,go,i;
    while(head[0] != '\n'){
        end = head;
        num = 0;
        go = 0;
		i = 0;
        while(end[0] != ' '&&end[0] != '\n'&&end[0] != '\0'){
			if(end[0] == '\\'){
				while(end[i] != '\0'){
					end[i] = end[i+1];
					i++;
				}
				i = 0;
			}
			end++;
			num++;
            go = 1;
        }
        if(go == 1){
            strncpy(parameter[parameter_num],head,num);
            parameter[parameter_num][num+1] = '\0';
            parameter_num++;
        }
        while(end[0] == ' '||end[0] == '\0'){
            end++;
        }
        head = end;
    }
    return parameter_num;
}

int go_cmd(char *buffer,int parameter_num,char parameter[100][256]){
    char str[15] = "--color=auto";
    int temp;
    unsigned short i;
    char *cmd[parameter_num+1];
    for(i = 0;i < parameter_num;i++){
        cmd[i] = parameter[i];
    }
    cmd[i] = NULL;

	if(cmd_history(cmd) == 0){
		return 1;
	}
	if(cmd_help(cmd) == 0){
		return 1;
	}
	if(cmd_hiscln(cmd) == 0){
		return 1;
	}
	if(cmd_cd(cmd) == 0){
		return 1;
	}
	if(analyze_cmd_twice(cmd) != 0){
		return 1;
	}

	if(check_cmd(cmd[0]) == 0){
		if(strcmp(cmd[0],"ls") == 0){
            cmd[i] = str;
            cmd[i+1] = NULL;
        }
        pid_t pid;
		if(strcmp(cmd[0],"vim") == 0||strcmp(cmd[0],"vi") == 0){
			pid = fork();
			if(pid == 0){
				pid = fork();
				if(pid == 0){
					signal(SIGINT,SIG_DFL);
					execvp(cmd[0],cmd);
					exit(0);
				}
				wait(&temp);
				exit(0);
			}
			wait(&temp);
		}
		else{
			pid = fork();
			if(pid == 0){
				signal(SIGINT,SIG_DFL);
				execvp(cmd[0],cmd);
				exit(0);
			}
		}
        wait(&temp);
    }
    return 0;
}

unsigned short analyze_cmd_twice(char *cmd[25]){
	unsigned short i = 0,state = 0;//state == 1 is '<';state == 2 is '>';state == 3 is '|';state == 4 is '&'
	while(cmd[i] != NULL){
		if(cmd[i][0] == '<'&&cmd[i][1] == '\0'){
			cmd[i] = NULL;
			state = 1;
			cmd_inredirection(i,cmd);
			break;
		}
		else if(cmd[i][0] == '>'&&cmd[i][1] == '\0'){
			cmd[i] = NULL;
			state = 2;
			cmd_outredirection(i,cmd);
			break;
		}
		i++;
	}
	return state;
}

int cmd_inredirection(unsigned short num,char *cmd[25]){
	pid_t pid;
	int fd,temp;
	char *file;
	file = cmd[num+1];
	pid = fork();
	if(pid == 0){
		if(check_cmd(cmd[0]) == 0){
			fd = open(file,O_RDONLY);
			dup2(fd,0);
			signal(SIGINT,SIG_DFL);
			execvp(cmd[0],cmd);
			exit(0);
		}
	}
	wait(&temp);
	return 0;
}

int cmd_outredirection(unsigned short num,char *cmd[25]){
	pid_t pid;
	int fd,temp;
	char *file;
	file = cmd[num+1];
	pid = fork();
	if(pid == 0){
		if(check_cmd(cmd[0]) == 0){
			fd = open(file,O_RDWR|O_CREAT|O_TRUNC,0644);
			dup2(fd,1);
			signal(SIGINT,SIG_DFL);
			execvp(cmd[0],cmd);
			exit(0);
		}
	}
	wait(&temp);
	return 0;
}

int cmd_piping(){
	;
}

int cmd_backstage(){
	;
}

int cmd_history(char *cmd[10]){
	char temp;
	FILE *fp;
	char home[50];
    struct passwd *usrname;
	if(strcmp(cmd[0],"history") == 0&&getuid() != 0){
		usrname = getpwuid(getuid());
		strcpy(home,"/home/");
		strcat(home,usrname->pw_name);
		strcat(home,"/history.txt");
	}
	else{
		return -1;
	}
	fp = fopen(home,"r");
	while(temp = fgetc(fp)){
		if(temp == EOF){
			break;
		}
		printf("%c",temp);
	}
	fclose(fp);
	return 0;
}

int cmd_help(char *cmd[1]){
	if(strcmp(cmd[0],"help") == 0){
		printf("\033[1;3;44m");
		printf("\n#This is a shell\n");
		printf("\033[0m");
		printf("\033[1;37;40m");
		printf("It built-in the following command:\n1.help:\n");
		printf("This command is display the information about this shell on your screen.\n");
		printf("2.history:\n");
		printf("This command is display the command you had used on your screen.\n");
		printf("3.histcln:\n");
		printf("This command is clean your history records about the command you had used.\n");
		printf("4.cd:\n");
		printf("This command will change the working directory which you places.\n");
		printf("\033[0m");
		printf("\033[1;3;44m");
		printf("#Thanks to use this shell\n");
		printf("\033[0m");
		printf("\033[1;31;46m");
		printf("Power by Monkery:Version 1.0-bete\n\n");
		printf("\033[0m");
	}
	else{
		return -1;
	}
	return 0;
}

int cmd_hiscln(char *cmd[1]){
	char home[50];
    struct passwd *usrname;
	if(strcmp(cmd[0],"histcln") == 0&&getuid() != 0){
		usrname = getpwuid(getuid());
		strcpy(home,"/home/");
		strcat(home,usrname->pw_name);
		strcat(home,"/history.txt");
		fopen(home,"w");
	}
	else{
		return -1;
	}
	return 0;
}

int cmd_cd(char *cmd[2]){
	char home[50];
	char path[100];
	char *now;
    struct passwd *usrname;
	memset(path,0,100);
	now = getcwd(NULL,0);
	if(strcmp(cmd[0],"cd") == 0){
		if(cmd[1] == NULL){
			usrname = getpwuid(getuid());
			strcpy(home,"/home/");
			strcat(home,usrname->pw_name);
			strcpy(path,home);
		}
		else if(cmd[1][0] == '.'&&cmd[1][1] == '/'){
			cmd[1]++;
			strcat(path,now);
			strcat(path,cmd[1]);
		}
		else if(cmd[1][0] == '.'&&cmd[1][1] == '.'&&cmd[1][2] == '\0'){
			strcat(path,now);
			strcat(path,"/");
			strcat(path,cmd[1]);
		}
		else if(cmd[1][0] == '/'){
			strcat(path,cmd[1]);
		}
		else if(cmd[1][0] == '~'&&cmd[1][1] == '\0'){
			usrname = getpwuid(getuid());
			strcpy(home,"/home/");
			strcat(home,usrname->pw_name);
			strcpy(path,home);
		}
		else if(cmd[1][0] == '~'&&cmd[1][1] != '\0'){
			cmd[1]++;
			usrname = getpwuid(getuid());
			strcpy(home,"/home/");
			strcat(home,usrname->pw_name);
			strcat(path,home);
			strcat(path,cmd[1]);
		}
		else{
			strcat(path,now);
			strcat(path,"/");
			strcat(path,cmd[1]);
		}
		if(chdir(path) == -1){
			printf("没有那个目录或文件!\n");
			return 0;
		}
	}
	else{
		return -1;
	}
	return 0;
}

int check_cmd(char *temp){
    unsigned short i = 0;
    DIR *dir;
    struct dirent *info;
    char *path[] = {"/bin","/usr/bin","./",NULL};
    if(temp[0] == '.'&&temp[1] == '/'){
        temp = temp+2;
    }
    while(path[i] != NULL){
        if((dir = opendir(path[i])) == NULL){
            printf("无法打开命令所在目录!\n");
            break;
        }
        else{
            while((info = readdir(dir)) != NULL){
                if(strcmp(temp,info->d_name) == 0){
                    closedir(dir);
                    return 0;
                }
            }
        }
        closedir(dir);
        i++;
    }
    printf("没有找到您要执行的命令!(或者没有支持)\n");
    return -1;
}
