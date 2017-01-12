#ifndef __CHANNEL_MGR_H__
#define __CHANNEL_MGR_H__

#include <set>
#include <string>
#include <muduo/net/TcpClient.h>
#include <muduo/net/InetAddress.h>

#include "BgMessageQueue.h"

using namespace std;
using namespace muduo::net;

namespace Gate{
    class GateServer;
}

class ServiceMgr;

class Service
{
public:
   Service(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr, const std::string& id, ServiceMgr* mgr);
   ~Service();

   bool Accept(unsigned int cmd);
   bool Push(const SSPacket& packet, const muduo::net::Buffer& buf);
   void onConnection(const muduo::net::TcpConnectionPtr& conn);

private:
    muduo::net::TcpClient client_;
    muduo::net::EventLoop* loop_;
    set<unsigned int> cmds_;
};
typedef std::shared_ptr<Service> ServicePtr;

class ServiceMgr
{
public:
    ~ServiceMgr();
    static ServiceMgr* Instance();

    int Init(Gate::GateServer* svr, muduo::net::EventLoop* loop);
    int Push(const SSPacket& header, const muduo::net::Buffer& buf);
    int Push(unsigned int channelId, const SSPacket& header, const muduo::net::Buffer& buf);
    void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time);

private:
    map<unsigned int, unique_ptr<Service> > channels_;
    muduo::net::EventLoop* loop_;
    ServiceMgr();
};


#endif
