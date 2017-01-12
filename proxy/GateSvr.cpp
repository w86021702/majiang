#include "GateSvr.h"
#include "Dispather.h"

#include <muduo/base/Logging.h>
#include "RequestDef.h"
#include <sstream>
#include "BgMessageQueue.h"
#include "ChannelMgr.h"

using namespace muduo;
using namespace muduo::net;
using namespace Gate;


GateServer::GateServer(EventLoop* loop,
        const InetAddress& listenAddr,
        int maxConnections)
    : server_(loop, listenAddr, "GateSvr"),
    numConnected_(0),
    nextConnId_(0),
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
    server_.setThreadNum(5);
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
            LOG_INFO << "numConnected = " << numConnected_ << " shutdown delay";
            return ;
        }

        auto* clientHandler = new CClient();
        clientHandler->connId = ++nextConnId_;
        clientHandler->conn = conn;
        connId2client_[++nextConnId_] = std::shared_ptr<CClient>(clientHandler);
        name2connId_[conn->name().c_str()] = nextConnId_;
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
    while (buf->readableBytes() >= CSPacket_Size)
    {
        const CSPacket * packet = (CSPacket *)buf->peek();
        uint32_t len = muduo::net::sockets::networkToHost32(packet->len);
        //uint32_t len = packet->len; 
        //const uint32_t len = muduo::net::sockets::hostToNetwork32(packet->len);
        LOG_INFO << "not full, packet recv cmd: " << packet->cmd  << " trans len: " << len
            << " packet_len: " << packet->len;
        if (len + CSPacket_Size <= buf->readableBytes())
        {
			LOG_INFO << "packet recv cmd: " << packet->cmd  << "  len: " << len;

            struct SSPacket header;
            //r.header = *packet;
            auto connIdIt = name2connId_.find(conn->name().c_str());
            if (connIdIt != name2connId_.end())
            {
                header.uid = 0;
                header.clientId = connIdIt->second;
                header.cmd = packet->cmd;
                header.type = EPacket_Type_INPUT;
                header.len = muduo::net::sockets::hostToNetwork32(len);
                header.time = packet->time;

                Buffer tmpBuf;
                tmpBuf.append(buf->peek() + CSPacket_Size, len);
                ServiceMgr::Instance()->Push(1, header, tmpBuf);
            }
            buf->retrieve(len + CSPacket_Size);
			//BgMessageQueue::Instance()->Push(r);
        }
		else
		{
			break;
		}
    }
}

void GateServer::notify(unsigned int clientId, muduo::net::Buffer* buf)
{
    auto connIt = connId2client_.find(clientId);
    assert(connIt != connId2client_.end());
    connIt->second->conn->send(buf);
}
