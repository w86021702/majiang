#ifndef __GATE_SVR_H__
#define __GATE_SVR_H__

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>

using namespace muduo;
using namespace muduo::net;


namespace Gate{

struct CClient
{
    unsigned int connId;
    muduo::net::TcpConnectionPtr conn;
};

class GateServer
{
public:
    GateServer(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& listenAddr,
            int maxConnections);

    void start();

private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);

    void onMessage(const muduo::net::TcpConnectionPtr& conn,
            muduo::net::Buffer* buf,
            muduo::Timestamp time);

    muduo::net::TcpServer server_;
    unsigned int numConnected_; // should be atomic_int
    unsigned int nextConnId_;
    const int kMaxConnections_;
    muduo::net::EventLoop* loop_;
    std::map<std::string, unsigned int> name2connId_;
    std::map<unsigned int, std::unique_ptr<CClient> > connId2client_;
};
}

#endif
