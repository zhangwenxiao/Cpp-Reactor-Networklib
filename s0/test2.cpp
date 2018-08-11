#include "EventLoop.h"
#include <muduo/base/Thread.h>

EventLoop* g_loop;

void threadFunc()
{
    g_loop -> loop();
}

int main()
{
    EventLoop loop;
    g_loop = &loop;
    muduo::Thread t(threadFunc);
    t.start();
    t.join();
}
