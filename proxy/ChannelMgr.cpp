#include "ChannelMgr.h"
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

Channel::Channel(muduo::net::EventLoop* loop, const InetAddress& listenAddr, const string& id)
    : loop_(loop),
      client_(loop, listenAddr, "EchoClient"+id)
{
    client_.setConnectionCallback(
            std::bind(&Channel::onConnection, this, std::placeholders::_1));
    client_.setMessageCallback(
            std::bind(&Channel::onMessage, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3));
}

Channel::~Channel()
{
}

void Channel::onConnection(const TcpConnectionPtr& conn)
{
    LOG_TRACE << conn->localAddress().toIpPort() << " -> "
        << conn->peerAddress().toIpPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");

    if (conn->connected())
    {
      ++current;
      if (implicit_cast<size_t>(current) < clients.size())
      {
        clients[current].get()->connect();
      }
      LOG_INFO << "*** connected " << current;
    }
    conn->send("world\n");
}

void Channel::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
{
    //找到对应的uid socket
    conn->send(buf->retrieveAllAsString());
}
