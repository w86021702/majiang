#ifndef __WORKER_H__
#define __WORKER_H__

class Worker
{
public:
    Worker();
    ~Worker();

    int init();

private:
    void threadFunc(void* arg);
};


#endif
