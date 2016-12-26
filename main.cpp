#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>

using namespace muduo;
using namespace muduo::net;

int main(int args, char **argv)
{
    EventLoop loop;
    loop.loop();

    return 0;
}
