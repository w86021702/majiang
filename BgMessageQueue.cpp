#include "BgMessageQueue.h"

BgMessageQueue* BgMessageQueue::Instance()
{
    static BgMessageQueue ins;
    return &ins;
}

int BgMessageQueue::Push(const MQRecord& r)
{
    queue_.put(r);
    return 0;
}

int BgMessageQueue::Pop(MQRecord& r)
{
    r = queue_.take();
    return 0;
}
