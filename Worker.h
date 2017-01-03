#ifndef __WORKER_H__
#define __WORKER_H__

#include<vector>
#include <muduo/base/Thread.h>

class Worker
{
public:
    Worker(uint32_t cnt = 4);
    ~Worker();

    int start();

private:
    void threadFunc();

	uint32_t threadCnt_;
	std::vector<std::unique_ptr<muduo::Thread>> threads_;
};


#endif
