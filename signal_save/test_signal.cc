#include <iostream>
#include <unistd.h>
#include <signal.h>



void PrintPending(const sigset_t &pending)
{
    for (int signo = 31; signo > 0; signo--)
    {
        if (sigismember(&pending, signo))
        {
            std::cout << "1";
        }
        else
        {
            std::cout << "0";
        }
    }
    std::cout << std::endl;
}

void handler(int signo)
{
    sigset_t pending;
    std::cout<<"############################"<<std::endl;
    sigpending(&pending);
    PrintPending(pending);
    std::cout << "handler " << signo << std::endl;
    // exit(100);
}


int main()
{

    signal(2,handler);

    while(true)
    {
        sleep(1);
    }
    // // 1.屏蔽二号信号
    // std::cout<<"getpid:"<<getpid()<<std::endl;
    // sigset_t set, oset;
    // sigemptyset(&set);
    // sigemptyset(&oset);
    // sigaddset(&set, 2);

    // //屏蔽
    // sigprocmask(SIG_BLOCK, &set, &oset);

    // // 2.让信号不断获取当前进程的pending信号集
    // int cnt = 0;
    // sigset_t pending;
    // while (true)
    // {
    //     sigpending(&pending);
    //     PrintPending(pending);

    //     sleep(1);
    //     cnt++;

    //     if(cnt == 5)
    //     {
    //         //运行15秒，解除对2号信号的屏蔽
    //         //将原先的set信号集覆盖当前的set信号集
    //         sigprocmask(SIG_SETMASK,&oset,nullptr);
    //         std::cout<<"2号信号的屏蔽已解除，准备递达"<<std::endl;
    //     }
    // }

    // std::cout << "getpid:" << getpid() << std::endl;
    // signal(2,handler);

    // sigset_t block,oblock;

    // sigemptyset(&block);
    // sigemptyset(&oblock);

    // sigaddset(&block,2);   //仅仅相当于自己定义了个局部变量，并对其设置，还没设置到OS中

    // sigprocmask(SIG_BLOCK,&block,&oblock); // 设置到OS中，屏蔽2号信号

    // while(true)
    // {
    //     sleep(1);
    // }

    // std::cout << "getpid:" << getpid() << std::endl;

    // signal(2, handler);
    // sleep(10);

    // // SIG_DFL 恢复信号捕捉
    //  signal(2, SIG_DFL);

    // // 忽视一个信号
    // signal(2, SIG_IGN);

    // std::cout << "getpid:" << getpid() << std::endl;
    // std::cout << "getpid:" << getpid() << std::endl;
    // std::cout << "getpid:" << getpid() << std::endl;
    // std::cout << "getpid:" << getpid() << std::endl;
    // while (true)
    // {
    //     sleep(1);
    // }
    // std::cout << "getpid:" << getpid() << std::endl;
    // int a = 10;
    // a /= 0;
    // std::cout << "getpid:" << getpid() << std::endl;
    return 0;
}