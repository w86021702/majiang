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
    string msg(buf->retrieveAllAsString());
    LOG_TRACE << conn->name() << " recv " << msg.size() << " bytes at " << time.toString();
    if (msg == "quit\n")
    {
      conn->send("bye\n");
      conn->shutdown();
    }
    else if (msg == "shutdown\n")
    {
      loop_->quit();
    }
    else
    {
      conn->send(msg);
    }
}
