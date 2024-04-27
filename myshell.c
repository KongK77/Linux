#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#define NUM 1024
#define SIZE 64
#define SEP " "
//#define DEBUG 1
char cwd[1024];
const char *getUsername()
{
    const char *name = getenv("USER");
    if(name) return name;
    else return NULL;
}
const char *getHostname()
{
    const char* hostname = getenv("HOSTNAME");
    if(hostname) return hostname;
    else return NULL;
}
const char *getCwd()
{
    const char* cwd = getenv("PWD");
    if(cwd) return cwd;
    else return NULL;
}
int getUserCommand(char *command,int num)
{
    printf("[%s@%s %s] $",getUsername(),getHostname(),getCwd());
    char * r = fgets(command,num,stdin);
    if(r == NULL) return -1;
    command[strlen(command)-1] = '\0';
    return strlen(command);
}
void commandSplit(char *in,char* out[])
{
    int argc = 0;
    out[argc++] = strtok(in,SEP);
    while(out[argc++] = strtok(NULL,SEP));
#ifdef DEBUG
    //用于测试 commandSplit是否生效，如果DEBUG被定义，则运行代码，未定义不运行
    for(int i = 0;out[i];i++)
    {
        printf("%d:%s\n",i,out[i]);
    }
#endif
}

int execute(char *argv[])
{
        pid_t id = fork();
        if(id < 0) return -1;
        else if(id == 0)  //child
        {
            //exec command
            execvp(argv[0],argv);
            exit(1);
        }
        else    //father
        {
            pid_t rid = waitpid(id,NULL,0);
            if(rid>0){}                 
        }
}
void cd(const char *path)
{
    chdir(path);
    char tmp[1024];
    getcwd(tmp,sizeof(tmp));
    sprintf(cwd,"PWD=%s",tmp);
    putenv(cwd);
}

//什么叫做内建命令：内建命令就是bash自己执行的，类似与自己内部的一个函数
//1->yes 0->no
int doBuildin(char *argv[])
{
    if(strcmp(argv[0],"cd") == 0)
    {
        char *path = NULL;
        if(argv[1] == NULL) path = ".";
        else path = argv[1];
        cd(path);
        return 1;
    }
    else if(0){}
    return 0;
}
int main()
{
    while(1)
    {
        char usercommand[NUM];
        char *argv[SIZE];
        //1.打印提示符&&获取用户命令字符串获取成功
        int n = getUserCommand(usercommand,sizeof(usercommand));
        if(n <= 0) continue;  //跳出循环
        //2.分割字符串
        commandSplit(usercommand,argv);
        //3.check build-in command
        n = doBuildin(argv);
        if(n) continue;
        //4.执行对应的命令
        execute(argv);
    }
}
