#ifndef __CHANNEL_MGR_H__
#define __CHANNEL_MGR_H__

#include <set>
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

   bool Accept(unsigned int cmd);
   bool Push(const Buffer& buf);

private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time);

    muduo::net::TcpClient client_;
    muduo::net::EventLoop* loop_;
    set<unsigned int> cmds_;
};

class ServiceMgr
{
public:
    ServiceMgr(muduo::net::EventLoop* loop);
    ~ServiceMgr();



private:
    map<unsigned int, unique_ptr<Channel> > channels_;
};


#endif
