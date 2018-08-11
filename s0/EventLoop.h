#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

#include <muduo/base/Thread.h>
#include <boost/noncopyable.hpp>

class EventLoop : boost::noncopyable {
public:

    EventLoop();
    ~EventLoop();

    void loop();
    
    void assertInLoopThread()
    {
        if(!isInLoopThread()) abortNotInLoopThread();
    }

    bool isInLoopThread() const
    {
        return threadId_ == muduo::CurrentThread::tid();
    }

    static EventLoop* getEventLoopOfCurrentThread();
private:

    void abortNotInLoopThread();

    bool looping_; // atomic
    const pid_t threadId_;
};

#endif
