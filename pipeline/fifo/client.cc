#include "comm.h"

using namespace std;

int main()
{
    int wfd = open(FILENAME, O_WRONLY);
    if (wfd < 0)
    {
        std::cerr << "errno:" << errno << ",errstring:" << strerror(errno) << std::endl;
        return 1;
    }
    std::cout<<"open fifo success .....write"<<std::endl;
    std::string message;
    while(true)
    {
        std::cout<<"Please Enter:";
        std::getline(std::cin,message);

        ssize_t s = write(wfd,message.c_str(),message.size());
        if(s < 0)
        {
            std::cerr << "errno:" << errno << ",errstring:" << strerror(errno) << std::endl;
            break;
        }
    }
    close(wfd);
    std::cout<<"close fifo success"<<std::endl;
    return 0;
}