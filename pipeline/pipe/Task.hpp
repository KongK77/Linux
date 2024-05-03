#pragma

//.hpp后缀文件：能把函数的声明和实现写在一起

#include <iostream>
#include <functional>
#include <vector>
#include <ctime>
#include <stdlib.h>
// using task_t = std::function<void()>;
typedef std::function<void()> task_t;

void DownLoad()
{
    std::cout << "我是一个下载任务"
              << "处理者: " << getpid() << std::endl;
}

void PrintLog()
{
    std::cout << "我是一个打印任务"
              << "处理者: " << getpid() << std::endl;
}

void PushVideoStream()
{
    std::cout << "我是一个推送视频流的任务"
              << "处理者: " << getpid() << std::endl;
}

class Init
{
public:
    // 任务码
    const static int g_download_code = 0;
    const static int g_printlog_code = 1;
    const static int g_push_video_stream_code = 2;
    // 任务集合
    std::vector<task_t> tasks;

public:
    Init()
    {
        tasks.push_back(DownLoad);
        tasks.push_back(PrintLog);
        tasks.push_back(PushVideoStream);

        srand(time(nullptr) ^ getpid());
    }
    bool CheckSafe(int code)
    {
        if (code >= 0 && code <= tasks.size())
            return true;
        else
            return false;
    }
    void RunTask(int code)
    {
        return tasks[code]();
    }
    int SelectTask()
    {
        int code = rand() % tasks.size();
        return code;
    }
    std::string ToDesc(int code)
    {
        switch (code)
        {
        case g_download_code:
            return "DownLoad";
        case g_printlog_code:
            return "PrintLog";
        case g_push_video_stream_code:
            return "PushVideoStream";
        default:
            return "NONE";
        }
    }
};

Init init; // 定义对象
