#include "GateSvr.h"
#include "Dispather.h"

#include <muduo/base/Logging.h>
#include "RequestDef.h"
#include <sstream>
#include "BgMessageQueue.h"

using namespace muduo;
using namespace muduo::net;
using namespace Gate;


GateServer::GateServer(EventLoop* loop,
        const InetAddress& listenAddr,
        int maxConnections)
    : server_(loop, listenAddr, "GateSvr"),
    numConnected_(0),
    kMaxConnections_(maxConnections),
    loop_(loop)

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
	LOG_INFO << "size : " << buf->readableBytes() << " packet recv : " << buf->peek();
    while (buf->readableBytes() >= SSPacket_Size)
    {
        const SSPacket * packet = (SSPacket *)buf->peek();
        const uint32_t len = packet->len;
        if (len + SSPacket_Size <= buf->readableBytes())
        {
			LOG_INFO << "packet recv cmd: " << packet->cmd << " uid:" << packet->uid << "  len: " << packet->len;

			MQRecord r;
			r.header = *packet;
            r.buf_.append(buf->peek() + SSPacket_Size, len);
            buf->retrieve(len + SSPacket_Size);
			BgMessageQueue::Instance()->Push(r);
            conn->send(string(buf->peek() + SSPacket_Size, len));
        }
		else
		{
			LOG_INFO << "body not full: " << packet->cmd << " uid:" << packet->uid << "  len: " << packet->len;
			break;
		}
    }

    //string msg(buf->retrieveAllAsString());
    //LOG_INFO << conn->name() << " echo " << msg.size() << " bytes at " << time.toString();
}
