#include <muduo/base/Logging.h>
#include "Dispather.h"


int CallBack::onMessage(
        const muduo::net::TcpConnectionPtr& conn,
        const muduo::net::Buffer* msg,
        muduo::Timestamp ts)
{
    int ret = func_(conn, msg, ts);
    return ret;
}

Dispather::Dispather()
{
}

Dispather::~Dispather()
{
}

int Dispather::registerMsg(uint32_t cmd, ICallBack* cb)
{
    assert(cb != NULL);
    cmd2cb_[cmd] = cb;
    return 0;
}

int Dispather::onMessage( 
        const muduo::net::TcpConnectionPtr& conn,
        const muduo::net::Buffer* msg,
        muduo::Timestamp ts)
{
    //解析出一个完整包分派下去
    return 0;
}
