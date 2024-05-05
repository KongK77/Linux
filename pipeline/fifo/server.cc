#include "comm.h"

bool Makefifo()
{
    int n = mkfifo(FILENAME, 0666);
    if (n < 0)
    {
        std::cerr << "errno:" << errno << ",errstring:" << strerror(errno) << std::endl;
        return false;
    }
    std::cout<<"mkfifo success .....read"<<std::endl;
    return true;
}


using namespace std;

int main()
{
Start:
    int rfd = open(FILENAME, O_RDONLY);
    if (rfd < 0)
    {
        std::cerr << "errno:" << errno << ",errstring:" << strerror(errno) << std::endl;
        if(Makefifo()) goto Start;
        else return 1;
    }
    std::cout<<"open fifo success .....read"<<std::endl;

    char buffer[1024];
    while (true)
    {
        ssize_t s = read(rfd, buffer, sizeof(buffer) - 1);
        if(s > 0)
        {
            buffer[s] = 0;
            std::cout<<"Client say:"<<buffer<<std::endl;
        }
        else if(s == 0)
        {
            std::cout<<"Client quit,server quit too!"<<std::endl;
            break;
        }
    }

    close(rfd);
    std::cout<<"close fifo success"<<std::endl;
    return 0;
}