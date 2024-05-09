#include <iostream>
#include <pthread.h>
#include <unistd.h>

int gcnt = 100;
void *ThreadRoutine(void *arg)
{
    const char *threadname = (const char *)arg;
    while (true)
    {
        std::cout << "I am a new thread,pid:" << getpid() << " gcnt:" << gcnt << " &gcnt:" << &gcnt << std::endl;
        gcnt--;
        sleep(1);
    }
}

int main()
{
    // 创建线程之前，已经有进程了

    pthread_t tid1;
    pthread_create(&tid1, nullptr, ThreadRoutine, (void *)"thread 1");

    // pthread_t tid2;

    // sleep(3);

    // pthread_create(&tid2, nullptr, ThreadRoutine, (void *)"thread 1");

    // sleep(3);
    // pthread_t tid3;

    // pthread_create(&tid3, nullptr, ThreadRoutine, (void *)"thread 1");
    // pthread_t tid4;

    // sleep(3);
    // pthread_create(&tid4, nullptr, ThreadRoutine, (void *)"thread 1");
    while (true)
    {
        std::cout << "I am main thread,pid:" << getpid() << " &gcnt:" << &gcnt << std::endl;
        sleep(1);
    }
    return 0;
}
