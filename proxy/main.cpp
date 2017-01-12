#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>

#include "GateSvr.h"
#include "Worker.h"
#include "ChannelMgr.h"

using namespace muduo;
using namespace muduo::net;


int main(int args, char **argv)
{
    uint32_t port = 2000;
    for (uint32_t i = 0; i < args; ++i)
    {
        if (strcmp(argv[i],"-p") == 0)
        {
            port = atoi(argv[++i]);
        } 
    }

    InetAddress addr(port);
    EventLoop loop;
    Gate::GateServer server(&loop, addr, 1024);
    server.start();

    ServiceMgr::Instance()->Init(&server, &loop);

	Worker worker;
	worker.start();

    loop.loop();

    return 0;
}
