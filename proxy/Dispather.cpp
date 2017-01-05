#include <muduo/base/Logging.h>
#include "Dispather.h"


Dispather::Dispather()
{
}

Dispather::~Dispather()
{
}

int CallBack::onMessage(
        const muduo::net::TcpConnectionPtr& conn,
        const muduo::net::Buffer* msg,
        muduo::Timestamp ts)
{
    int ret = func_(conn, msg, ts);
    return ret;
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
    uint32_t cmd = 0; 
    auto iter = cmd2cb_.find(cmd);
    ICallBack *cb = iter->second;
    cb->onMessage(conn, msg, ts);
    return 0;
}
