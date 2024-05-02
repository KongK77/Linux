#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<ctype.h>
#include<fcntl.h>
#include<sys/stat.h>

#define NUM 1024
#define SIZE 64
#define SEP " "
//#define DEBUG 1

#define NoneRedir 0
#define OutputRedir 1
#define AppendRedir 2
#define InputRedir 3

int redir = NoneRedir;
char *filename = NULL;

char cwd[1024];

char enval[1024];  // for test

int lastcode = 0; 


char *homepath()
{
    char *home = getenv("HOME");
    if(home)  return home;
    else
        return (char*)".";
}
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
        else if(id == 0)  //执行子进程
        {
            //程序替换会不会影响曾经的重定向呢？？ 不会，为什么？
            //
            int fd = 0;
            if(redir == InputRedir)
            {
                fd = open(filename,O_RDONLY);
                dup2(fd,0);
            }
            else if(redir == OutputRedir)
            {
                fd = open(filename,O_WRONLY|O_CREAT|O_TRUNC,0666);
                dup2(fd,1);
            }
            else if(redir == AppendRedir)
            {
                fd = open(filename,O_WRONLY|O_CREAT|O_APPEND,0666);
                dup2(fd,1);
            }
            else
            {
                //do noting
            }
            //程序替换
            execvp(argv[0],argv);
            exit(1);
        }
        else    //执行父进程
        {
            int status;
            pid_t rid = waitpid(id,&status,0);
            if(rid>0)
            {
                lastcode = WEXITSTATUS(status);
            }                 
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
//1->yes 0->no  -1->error
int doBuildin(char *argv[])
{
    if(strcmp(argv[0],"cd") == 0)
    {
        char *path = NULL;
        if(argv[1] == NULL) path = homepath();
        else path = argv[1];
        cd(path);
        return 1;
    }
    else if(strcmp(argv[0],"export") == 0)
    {
        if(argv[1] == NULL) return 1;
        strcpy(enval,argv[1]);
        putenv(enval);
        return 1;
    }
    else if(strcmp(argv[0],"echo") == 0)
    {
        if(argv[1] == NULL)
        {
            printf("\n");
            return 1;
        }
        if(*(argv[1]) == '$' && strlen(argv[1]) > 1)
        {
            char *val = argv[1]+1;
            if(strcmp(val,"?") == 0)  //如果是？ 打印最后一个执行的命令的状态退出码
            {
                printf("%d\n",lastcode);
                lastcode = 0;
            }
            else
            {
                const char *enval = getenv(val);
                if(enval) printf("%s\n",enval);
                else printf("\n");
            }
            return 1;
        }
        else
        {
            printf("%s\n",argv[1]);
            return 1;
        }
    }
    else if(0){}

    return 0;
}
#define SkipSpace(pos) do{ while(isspace(*pos))  pos++; }while(0)

void checkRedir(char usercommand[],int len)
{

    // ls -a -l < log.txt
    // ls -a -l > log.txt
    // ls -a -l >> log.txt
    char *end = usercommand + len - 1;
    char *start = usercommand;
    while(end > start)
    {
        if(*end == '>')  //覆盖写入
        {
            if(*(end - 1) == '>')//追加重定向
            {
                *(end-1) ='\0';
                filename = end+1;
                SkipSpace(filename);
                redir = AppendRedir;
                break;
            }
            else
            {
                *end = '\0';
                filename = end+1;
                SkipSpace(filename);
                redir = OutputRedir;
                break;
            }
        }
        else if(*end == '<')
        {
            *end = '\0';
            filename = end +1;  
            SkipSpace(filename); //如果有空格 就跳过
            redir = InputRedir;
            break;
        }
        else
        {
            end--;
        }
    }
}
int main()
{
    while(1)
    {
        redir = NoneRedir;
        filename = NULL;
        char usercommand[NUM];
        char *argv[SIZE];
        //1.打印提示符&&获取用户命令字符串获取成功
        int n = getUserCommand(usercommand,sizeof(usercommand));
        if(n <= 0) continue;  //跳出循环

        //1.检查是否发生重定向
        checkRedir(usercommand,strlen(usercommand));
        //2.分割字符串
        commandSplit(usercommand,argv);
        //3.check build-in command
        n = doBuildin(argv);
        if(n) continue;
        //4.执行对应的命令
        execute(argv);
    }
}
