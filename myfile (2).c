#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
int main()
{
    //将权限掩码设置为0
    umask(0);
    //int fd = open("log.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);
    int fd = open("log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);
    if(fd<0)
    {
        perror("open");
        return 1;
    }

    const char *msg ="cccccc\n";

    write(fd,msg,strlen(msg));




    close(fd);

    return 0;
}
