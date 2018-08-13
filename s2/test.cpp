#include "EventLoop.h"

#include <cstdio>

int cnt = 0;
muduo::EventLoop* g_loop;

void printTid()
{
    printf("pid = %d, tid = %d\n", getpid(), muduo::CurrentThread::tid());
    printf("now %s\n", muduo::Timestamp::now().toString().c_str());
}

void print(const char* msg)
{
    printf("msg %s %s\n", muduo::Timestamp::now().toString().c_str(), msg);
    if(++cnt == 20)
        g_loop -> quit();
}

int main()
{
    printTid();
    muduo::EventLoop loop;
    g_loop = &loop;

    print("main");
    loop.runAfter(1, [](){ print("once1"); });
    loop.runAfter(1.5, [](){ print("once1.5"); });
    loop.runAfter(2.5, [](){ print("once2.5"); });
    loop.runAfter(3.5, [](){ print("once3.5"); });
    loop.runEvery(2, [](){ print("every2"); });
    loop.runEvery(3, [](){ print("every3"); });

    loop.loop();
    print("main loop exits");
    sleep(1);
}

