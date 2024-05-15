#include <iostream>
#include <thread>
#include "Thread.hpp"
#include <vector>
#include <unistd.h>
#include <cstdio>
#include <mutex>
#include "LockGuard.hpp"

std::string GetThreadName()
{
    static int number = 1;
    char name[64];
    snprintf(name, sizeof(name), "thread-%d", number++);
    return name;
}

void Print(int num)
{
    while (num)
    {
        std::cout << "hello world : " << num-- << std::endl;
        sleep(1);
    }
}

class ThreadData
{
public:
    ThreadData(const std::string &threadname, pthread_mutex_t *mutex)
        : threadname(threadname), pmutex(mutex)
    {
    }
    ~ThreadData()
    {
    }

public:
    std::string threadname;
    pthread_mutex_t *pmutex;
};
// 全局共享资源
int ticket = 10000;
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 加锁：
// 1.我们要尽可能的给少的代码块加锁
// 2.一般加锁，都是给临界区加锁
void GetTicket(ThreadData *td)
{
    while (true)
    {
        // 3.根据互斥的定义，任何时刻，只允许一个进程申请锁成功！多个线程申请锁失败，失败的线程怎么办？在mutex上进程阻塞，本质就是等待！
        pthread_mutex_lock(td->pmutex); // 申请锁本身就是原子的、安全的
        {
            if (ticket > 0) // 4.一个线程在临界区中访问临界资源的时候，可能不能发生切换？
            {
                usleep(1000); // 充当抢票花费的时间
                printf(" get a ticket:%d \n", ticket);
                ticket--;
                pthread_mutex_unlock(td->pmutex);
            }
            else
            {
                pthread_mutex_unlock(td->pmutex);
                break;
            }
        }
    }
}
int main()
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);

    std::string name1 = GetThreadName();
    ThreadData *td = new ThreadData(name1, &mutex);
    Thread<ThreadData *> t1(name1, GetTicket, td);

    std::string name2 = GetThreadName();
    ThreadData *td2 = new ThreadData(name2, &mutex);
    Thread<ThreadData *> t2(name2, GetTicket, td2);

    std::string name3 = GetThreadName();
    ThreadData *td3 = new ThreadData(name3, &mutex);
    Thread<ThreadData *> t3(name3, GetTicket, td3);

    std::string name4 = GetThreadName();
    ThreadData *td4 = new ThreadData(name4, &mutex);
    Thread<ThreadData *> t4(name4, GetTicket, td4);

    t1.Start();
    t2.Start();
    t3.Start();
    t4.Start();

    t1.Join();
    t2.Join();
    t3.Join();
    t4.Join();

    pthread_mutex_destroy(&mutex);
    delete td;
    delete td2;
    delete td3;
    delete td4;


    // Thread<int> t(GetThreadName(),Print,10);

    // t.Start();

    // t.Join();

    // 这不就是 先描述，再组织
    //  const int num = 5;
    //  std::vector<Thread> threads;

    // for (int i = 0; i < num; i++)
    // {
    //     threads.push_back(Thread(Print, GetThreadName()));
    // }

    // for (auto &t : threads)
    // {
    //     std::cout << t.ThreadName() << ",is running: " << t.IsRunning() << std::endl;
    // }

    // for (auto &t : threads)
    // {
    //     t.Start();
    // }

    // for (auto &t : threads)
    // {
    //     std::cout << t.ThreadName() << ",is running: " << t.IsRunning() << std::endl;
    // }

    // for (auto &t : threads)
    // {
    //     t.Join();
    // }

    // Thread t(Print,GetThreadName());
    // std::cout<<"is thread running?"<<t.IsRunning()<<std::endl;

    // t.Start();

    // std::cout<<"is thread running?"<<t.IsRunning()<<std::endl;

    // t.Join();
    return 0;
}