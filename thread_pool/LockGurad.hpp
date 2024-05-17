#pragma once
#include<pthread.h>

class Mutex
{
public:
    Mutex(pthread_mutex_t *lock):_lock(lock)
    {}
    void Lock()
    {
        pthread_mutex_lock(_lock);
    }
    void Unlock()
    {
        pthread_mutex_unlock(_lock);    
    }
    ~Mutex()
    {}
private:
    pthread_mutex_t *_lock;
};

class LockGurad
{
public:
    LockGurad(pthread_mutex_t *lock):_mutex(lock)
    {
        _mutex.Lock();
    }
    ~LockGurad()
    {
        _mutex.Unlock();
    }
private:
    Mutex _mutex;
};