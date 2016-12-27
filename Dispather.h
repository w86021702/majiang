#ifndef __DISPATHER_H__
#define __DISPATHER_H__

#include <muduo/net/Callbacks.h>
#include <map>

class ICallBack
{
public:
    virtual int onMessage(
            const muduo::net::TcpConnectionPtr& conn,
            const muduo::net::Buffer* msg,
            muduo::Timestamp ts) = 0;
};

class CallBack : ICallBack
{
public:
    typedef std::function<int(const muduo::net::TcpConnectionPtr&,
            const muduo::net::Buffer*,
            muduo::Timestamp)> CallBackFunc;

    CallBack(const CallBackFunc& func):func_(func){}

    virtual int onMessage(
            const muduo::net::TcpConnectionPtr& conn,
            const muduo::net::Buffer* msg,
            muduo::Timestamp ts);

private:
    CallBackFunc func_;
};

class Dispather
{
public:
    Dispather();
    ~Dispather();

    int registerMsg(uint32_t cmd, ICallBack* cb);
    int onMessage( 
            const muduo::net::TcpConnectionPtr& conn,
            const muduo::net::Buffer* msg,
            muduo::Timestamp ts);

private:
    std::map<uint32_t, ICallBack*> cmd2cb_;

};


#endif
