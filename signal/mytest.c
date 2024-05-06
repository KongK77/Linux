#include<stdio.h>
#include<unistd.h>
#include<signal.h>


int cnt = 0;
int main()
{
    
    alarm(5);
    
    // while(1)
    // {
    //     printf("I am a process ,pid : %d\n",getpid());
    //     sleep(1);
    // }
    return 0;
}