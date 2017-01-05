#ifndef __CHANNEL_MGR_H__
#define __CHANNEL_MGR_H__

#include <string>
#include <muduo/net/TcoClient.h>
#include <muduo/net/InetAddress.h>
using namespace std;

class muduo::net::TcpEventLoop;

class Channel
{
public:
   Channel(muduo::net::EventLoop* loop, const InetAddress& listenAddr, const string& id);
   ~Channel();

private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time);

    muduo::net::TcpClient client_;
    muduo::net::EventLoop* loop_;
};

class ChannelMgr
{
public:
    ChannelMgr(muduo::net::EventLoop* loop);
    ~ChannelMgr();

    int ReadCfg(const string& path = "");


private:
    std::vector<std::unique_ptr<Channel>> channels_;
};


#endif
