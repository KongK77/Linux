#include <iostream>
#include <cassert>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>

#include "Task.hpp"

const int num = 5;
static int number = 1;
const bool g_always_loop = true;

class channel
{
public:
    channel(int fd, pid_t id) : ctrlfd(fd), workerid(id)
    {
        name = "channel" + std::to_string(number++);
    }

public:
    int ctrlfd;
    pid_t workerid;
    std::string name;
};

void Work()
{
    while (true)
    {
        int code = 0;
        ssize_t n = read(0, &code, sizeof(code));
        if (n == sizeof(code))
        {
            if (!init.CheckSafe(code))
                continue;
            init.RunTask(code);
        }
        else if (n == 0)
        {
            break;
        }
        else
        {
            // do nothing
        }
    }

    std::cout << "child quit" << std::endl;
}

void PrintFd(const std::vector<int> &fds)
{
    std::cout << "close fd : " << std::endl;
    for (auto fd : fds)
    {
        std::cout << fd << " ";
    }
    std::cout << std::endl;
}
// 传参形式
// 1.输入参数：const &
// 2.输出参数：*
// 3.输入输出参数：&
void CreateChannels(std::vector<channel> *c)
{

    std::vector<int> old;
    for (int i = 0; i < num; i++)
    {
        // 1.定义并创建管道
        int pipefd[2];
        int n = pipe(pipefd);
        assert(n == 0);
        (void)n;

        // 2.创建进程
        pid_t id = fork();
        assert(id != -1);

        // 3.构建单向通信信道
        if (id == 0) // child   only read
        {
            if (!old.empty())
            {
                for (auto fd : old)
                {
                    close(fd);
                }
                PrintFd(old);
            }
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            Work();
            exit(0);
        }

        // father    only write
        close(pipefd[0]);
        c->push_back(channel(pipefd[1], id));
        // childid,pipefd[1]
        old.push_back(pipefd[1]);
    }
}

void PrintDebug(const std::vector<channel> &c)
{
    for (const auto &channel : c)
    {
        std::cout << channel.name << "," << channel.ctrlfd << "," << channel.workerid << std::endl;
        sleep(1);
    }
}

void SendCommand(std::vector<channel> &channels, bool flag, int num = -1)
{
    int pos = 0;
    while (true)
    {
        // 1.选择任务
        int command = init.SelectTask();

        // 2.选择信道（进程）

        auto &c = channels[pos++];
        pos %= channels.size();

        // debug

        std::cout << "send command to :" << init.ToDesc(command) << "[" << command << "]"
                  << " in " << c.name << " worker is :" << c.workerid << std::endl;
        sleep(1);
        // 3.发送任务
        write(c.ctrlfd, &command, sizeof(command));

        // 4.判断是否要退出
        if (!flag)
        {
            num--;
            if (num <= 0)
                break;
        }
        // sleep(1);
    }

    std::cout << "SendCommand success!" << std::endl;
}

void ReleaseChannels(std::vector<channel> &c)
{
    // version 2
    int num = c.size() - 1;
    for (; num >= 0; num--)
    {
        close(c[num].ctrlfd);
        waitpid(c[num].workerid, nullptr, 0);
    }
    // version 1
    // for (const auto &channel : c)
    // {
    //     close(channel.ctrlfd);
    //     //waitpid(channel.workerid, nullptr, 0);
    // }
    // for (const auto &channel : c)
    // {
    //     pid_t rid = waitpid(channel.workerid, nullptr, 0);
    //     if (rid == channel.workerid)
    //     {
    //         std::cout << "wait child: " << channel.workerid << " success quit" << std::endl;
    //     }
    // }
}

int main()
{
    std::vector<channel> channels;
    // 创建信道，创建进程
    CreateChannels(&channels);

    // 开始发送任务
    SendCommand(channels, !g_always_loop, 10);

    // 回收资源，想让子进程退出，并且释放管道，只要关闭写端就行
    ReleaseChannels(channels);
    return 0;
}