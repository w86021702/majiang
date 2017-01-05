#ifndef __BG_MESSAGEQUEUE_H__
#define __BG_MESSAGEQUEUE_H__

#include "RequestDef.h"
#include <muduo/base/BlockingQueue.h>
#include <muduo/net/TcpServer.h>

struct MQRecord
{
    SSPacket header;
    muduo::net::Buffer buf_;
};

class BgMessageQueue
{
public:
    static BgMessageQueue* Instance();
    ~BgMessageQueue(){}

    int Push(const MQRecord& r);
    int Pop(MQRecord& r);


private:
    BgMessageQueue(){}
    muduo::BlockingQueue<MQRecord> queue_;
};

#endif
