#ifndef __EVENTLOOP_THREADPOOL_H__
#define __EVENTLOOP_THREADPOOL_H__

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <vector>

namespace muduo {

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : boost::noncopyable {
public:
    EventLoopThreadPool(EventLoop* baseLoop);
    ~EventLoopThreadPool();

    void setThreadNum(int numThreads)
    {
        numThreads_ = numThreads_;
    }

    void start();
    EventLoop* getNextLoop();

private:
    EventLoop* baseLoop_;
    bool started_;
    int numThreads_;
    int next_; // Always in loop thread
    boost::ptr_vector<EventLoopThread> threads_;
    std::vector<EventLoop*> loops_;
}; // class EventLoopThreadPool

} // namespace muduo

#endif
