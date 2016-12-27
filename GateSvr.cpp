#include "GateSvr.h"

#include <muduo/base/Logging.h>

using namespace muduo;
using namespace muduo::net;
using namespace Gate;


GateServer::GateServer(EventLoop* loop,
        const InetAddress& listenAddr,
        int maxConnections)
    : server_(loop, listenAddr, "GateSvr"),
    numConnected_(0),
    kMaxConnections_(maxConnections)
{
    server_.setConnectionCallback(
            std::bind(&GateServer::onConnection, this, std::placeholders::_1));
    server_.setMessageCallback(
            std::bind(&GateServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void GateServer::start()
{
    server_.start();
}

void GateServer::onConnection(const TcpConnectionPtr& conn)
{
    LOG_INFO << "GateSvr - " << conn->peerAddress().toIpPort() << " -> "
        << conn->localAddress().toIpPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");

    if (conn->connected())
    {
        ++numConnected_;
        if (numConnected_ > kMaxConnections_)
        {
            conn->shutdown();
            conn->forceCloseWithDelay(3.0);  // > round trip of the whole Internet.
        }
    }
    else
    {
        --numConnected_;
    }
    LOG_INFO << "numConnected = " << numConnected_;
}

void GateServer::onMessage(const TcpConnectionPtr& conn,
        Buffer* buf,
        Timestamp time)
{
    string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " echo " << msg.size() << " bytes at " << time.toString();
    conn->send(msg);
}
