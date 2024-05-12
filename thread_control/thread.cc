#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <functional>
#include <time.h>
#include <vector>

struct data
{
    int x;
    int y;
    char op;
};

struct res
{
    int code;
    int result;
};
class InAndOut
{
    std::vector<data> datas;
    std::vector<res> results;
};

class pthreadReturn
{
public:
    pthreadReturn(pthread_t id, std::string info, int code)
        : id(id), info(info), code(code)
    {
    }

public:
    pthread_t id;
    std::string info;
    int code;
};

std::string ToHex(pthread_t tid)
{
    char id[64];
    snprintf(id, sizeof(id), "0x%lx", tid);

    return id;
}
void *threadRoutine(void *arg)
{
    int cnt = 5;

    while (true)
    {
        std::cout << " new thread is running" << std::endl;
        sleep(1);
    }

    return nullptr;
    // usleep(1000);
    // std::string name = static_cast<const char *>(arg);

    // int cnt = 5;
    // while (cnt--)
    // {
    //     sleep(1);
    //     std::cout << "new thread:" << name << " new thread tid:" << ToHex(pthread_self()) << std::endl;
    // }

    // pthreadReturn *ret = new pthreadReturn(pthread_self(), "thread quit normal", 10);
    // return ret;

    // return (void*)"thread-1 done"; // 返回这个字符串常量的起始地址

    // return nullptr; // 1.第一种终止线程的方式
    // exit(13);  // 会让整个进程终止

    // pthread_exit(nullptr);  // 2.第二种终止线程的方式
}

// thread id 本质就是一个地址
// 获取返回值
int main()
{

    pthread_t tid;
    pthread_create(&tid, nullptr, threadRoutine, (void *)"thread-1");
    sleep(5);
    // pthread_detach(tid)
    int n = pthread_cancel(tid);
    std::cout << "main thread cancel down n:" << n << std::endl;

    void *ret = nullptr;
    n = pthread_join(tid, &ret);
    std::cout << "main thread down n:" << n << "  thread return:" << (int64_t)ret << std::endl;
    // #define PTHREAD_CANCELED  ((void*)-1)

    // 线程是可以被设置为分离状态的
    // 线程模式joinable的
    // std::cout << "main thread,"
    //           << " main thread tid:" << ToHex(tid) << std::endl;
    // sleep(10);

    // // 线程默认要等待退出
    // // 1.线程退出，没有等待，会导致僵尸状态
    // // 2.线程退出时，主线程如何获取新线程的返回值?

    // void *ret = nullptr;
    // int n = pthread_join(tid, &ret);
    // pthreadReturn *r = static_cast<pthreadReturn *>(ret);

    // std::cout << "new thread return: " << r->id << " info:" << r->info << " quit code:" << r->code << std::endl;

    // sleep(5);
    return 0;
}

// // typedef std::function<void()> func_t;
// using func_t = std::function<void()>;
// const size_t threadnum = 5;

// class ThreadData
// {
// public:
//     ThreadData(const std::string &name, const uint64_t &ctime, func_t f)
//         : threadname(name), createtime(ctime), func(f)
//     {
//     }

// public:
//     std::string threadname;
//     uint64_t createtime;
//     func_t func;
// };

// void Print()
// {
//     std::cout << "我是线程执行的大任务的一部分" << std::endl;
// }
// // 新线程
// void *ThreadRoutine(void *args)
// {
//     ThreadData *td = static_cast<ThreadData *>(args);

//     int a = 10;
//     while (true)
//     {
//         std::cout << "a new thread name :" << td->threadname << " create time :" << td->createtime << std::endl;
//         td->func();

//         // if(td->threadname == "thread-4")
//         // {
//         //     std::cout<<td->threadname<<"触发了异常"<<std::endl;
//         //     a/=0;    //故意制造异常
//         // }
//         sleep(1);
//     }
// }

// //

// // 如何给线程传参
// // 获取返回值
// int main()
// {
//     std::vector<pthread_t> pthreads;
//     for (size_t i = 1; i <= threadnum; i++)
//     {
//         //std::string threadname = std::to_string(i);
//         char threadname[64];
//         snprintf(threadname,sizeof(threadname),"%s-%lu","thread",i);

//         pthread_t tid;
//         ThreadData *td = new ThreadData(threadname, (uint64_t)time(nullptr), Print);
//         pthread_create(&tid, nullptr, ThreadRoutine, td);
//         pthreads.push_back(tid);

//         sleep(1);
//     }
//     std::cout<<"tid:";
//     for(const auto &tid : pthreads)
//     {
//         std::cout<<tid<<std::endl;
//     }

//     while (true)
//     {
//         std::cout << "main thread" << std::endl;
//         sleep(3);
//     }
//     return 0;
// }