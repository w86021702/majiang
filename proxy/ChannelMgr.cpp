#include "ChannelMgr.h"
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

using namespace std;
using namespace muduo::net;

Service::Service(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr, const string& id, ServiceMgr* mgr)
    : loop_(loop),
    client_(loop, listenAddr, id.c_str())
{
    client_.setConnectionCallback(
            std::bind(&Service::onConnection, this, std::placeholders::_1));
    client_.setMessageCallback(
            std::bind(&ServiceMgr::onMessage, mgr, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3));

    client_.connect();
}

Service::~Service()
{
}

void Service::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
    LOG_TRACE << conn->localAddress().toIpPort() << " -> "
        << conn->peerAddress().toIpPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");

    if (conn->connected())
    {
        LOG_INFO << "*** connected ";
    }
}

bool Service::Accept( unsigned int cmd)
{
    return true;
}

bool Service::Push(const SSPacket& packet, const muduo::net::Buffer& buf)
{
    if (client_.connection())
    {
        client_.connection()->send((const char*)&packet, sizeof(packet));
        muduo::net::Buffer tmpBuf = buf;
        client_.connection()->send(&tmpBuf);
    }

    return true;
}

ServiceMgr* ServiceMgr::Instance()
{
    static ServiceMgr mgr;
    return &mgr;
}


ServiceMgr::ServiceMgr()
{
}

ServiceMgr::~ServiceMgr()
{
}

int ServiceMgr::Init(Gate::GateServer* svr, muduo::net::EventLoop* loop)
{
    loop_ = loop;

    channels_[1] = std::unique_ptr<Service>(new
            Service(loop_, muduo::net::InetAddress("192.168.8.59", 9094), "1", this));
    return 0;
}

void ServiceMgr::onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time)
{
    //找到对应的uid socket
	LOG_INFO << "size : " << buf->readableBytes() << " packet recv : " << buf->peek();
    while (buf->readableBytes() >= SSPacket_Size)
    {
        const SSPacket * packet = (SSPacket *)buf->peek();
        const uint32_t len = muduo::net::sockets::networkToHost32(packet->len);
        if (len + SSPacket_Size <= buf->readableBytes())
        {
			LOG_INFO << "packet recv cmd: " << packet->cmd  << "  len: " << packet->len;

            struct CSPacket header;
			//r.header = *packet;
            header.cmd = packet->cmd;
            header.checksum = 0;
            header.len = muduo::net::sockets::hostToNetwork32(len);
            header.time = packet->time;

            Buffer tmpBuf;
            tmpBuf.append((char*)&header, sizeof(header));
            tmpBuf.append(buf->peek() + SSPacket_Size, len);
            buf->retrieve(len + CSPacket_Size);
			//BgMessageQueue::Instance()->Push(r);
        }
		else
		{
			break;
		}
    }
}


int ServiceMgr::Push(const SSPacket& packet, const muduo::net::Buffer& buf)
{
    return 0;
}

int ServiceMgr::Push(unsigned int channelId, const SSPacket& r, const muduo::net::Buffer& buf)
{
    auto iter = channels_.find(channelId);
    iter->second->Push(r, buf);

    return 0;
}
