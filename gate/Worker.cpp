#include "proto/cmddef.h"
#include "Worker.h"
#include "BgMessageQueue.h"

Worker::Worker(uint32_t cnt)
	:threadCnt_(cnt)
{
}

Worker::~Worker()
{
}

void Worker::threadFunc()
{
    printf("tid=%d, %s started\n",
           muduo::CurrentThread::tid(),
           muduo::CurrentThread::name());

	bool running = true;
	while(running)
	{
		MQRecord r;
		if (0 != BgMessageQueue::Instance()->Pop(r))
		{
			printf("pop error \n");
			continue;
		}

        if (r.header.cmd == CMD_LOGIN)
        {
            printf("uid:%d tid=%d queue cmd : %u, parse packet \n", r.header.uid, muduo::CurrentThread::tid(), r.header.cmd);
        }
        else
        {
            printf("uid:%d tid=%d queue result %s\n", r.header.uid, muduo::CurrentThread::tid(), r.buf_.peek());
        }
	}
}

int Worker::start()
{
	for (uint32_t i = 0; i < threadCnt_; ++i)
	{
		threads_.push_back(
				std::unique_ptr<muduo::Thread>(new muduo::Thread(
						std::bind(&Worker::threadFunc, this), muduo::string("worker"))
					));
	}

	for_each(threads_.begin(), threads_.end(), std::bind(&muduo::Thread::start, std::placeholders::_1));

    return 0;
}
