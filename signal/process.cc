#include <iostream>
#include <unistd.h>
#include <signal.h>

int cnt = 0;

void handler(int signo)
{
    int n = alarm(2);
    std::cout << "result:" << n << std::endl;
    // std::cout << " get a signo " << signo << " alarm:" << cnt++ << std::endl;
    // exit(0);
}

int main()
{
    signal(14, handler);
    
    alarm(2);

    while (true)
    {
        sleep(1);
    }
    // signal(8,handler);
    // int a = 10;
    // a /= 0;

    // int *p = nullptr;
    // *p = 100;

    // signal(SIGABRT, handler);  //自定义处理信号

    // abort();

    // while(1)
    // {
    //     std::cout<<"I am running"<<std::endl;
    //     sleep(1);
    // }

    // while(1)
    // {
    //     raise(2); //给自己发信号
    //     sleep(1);
    // }

    return 0;
}
// static void Usage(const std::string &proc)
// {
//     std::cout<<"\nUsage: "<<proc<<" signumber process"<<std::endl;
// }
// void handler(int signo)
// {
//     std::cout << "获得一个" << signo << "信号" << std::endl;
//     // exit(1);
// }
// int main(int argc,char *argv[])
// {
//     if(argc != 3)
//     {
//         Usage(argv[0]);
//         exit(0);
//     }

//     int signumber = std::stoi(argv[1]+1);
//     int processid = std::stoi(argv[2]);

//     kill(processid,signumber);

//     // signal(2, handler);
//     // signal(19, handler);
//     // signal(20, handler);
//     // signal(3, handler);

//     // signal(9, handler);  // 9号信号不能被自定义捕捉
//     // while (true)
//     // {
//     //     std::cout << "running....,pid:" << getpid() << std::endl;
//     //     sleep(1);
//     // }
//     return 0;
// }