#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <pthread.h>

template<class T>
using func_t = std::function<void(T)>;
// typedef std::function<void()> func_t;


template<class T>
class Thread
{
public:
    Thread(const std::string &threadname,func_t<T> func,T data)
        : _tid(0), _threadname(threadname), _isrunning(false), _func(func),_data(data)
    {
    }
    static void *ThreadRoutine(void *args)  // 类内方法，需要用static去掉this指针
    {
        //(void)argv;  // 仅仅是为了防止编译器有报警
        Thread *ts = static_cast<Thread *>(args);
        ts->_func(ts->_data);

        return nullptr;
    }
    bool Start()
    {
        int n = pthread_create(&_tid, nullptr, ThreadRoutine, this);
        if (n == 0)
        {
            _isrunning = true;
            return true;
        }
        else
            return false;
    }
    bool Join()
    {
        if (!_isrunning)
            return true;
        int n = pthread_join(_tid, nullptr);
        if (n == 0)
        {
            _isrunning = false;
            return true;
        }
        return false;
    }
    bool IsRunning()
    {
        return _isrunning;
    }
    std::string ThreadName()
    {
        return _threadname;
    }
    ~Thread()
    {
    }

private:
    pthread_t _tid;          // id
    std::string _threadname; // 名字
    bool _isrunning;         // 是否在运行
    func_t<T> _func;
    T _data;
};