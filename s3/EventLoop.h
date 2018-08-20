#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

#include "Callbacks.h"
#include "TimerId.h"

#include <muduo/base/Thread.h>
#include <muduo/base/Timestamp.h>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <vector>

namespace muduo {
class Channel;
class Poller;
class TimerQueue;

class EventLoop : boost::noncopyable {
public:

    using Functor = boost::function<void()>;

    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel* channel);
    void quit();

    // Time when poll returns, usually means data arrival
    Timestamp pollReturnTime() const { return pollReturnTime_; }

    TimerId runAt(const Timestamp& time, const TimerCallback& cb);
    TimerId runAfter(double delay, const TimerCallback& cb);
    TimerId runEvery(double interval, const TimerCallback& cb);
    
    void assertInLoopThread()
    {
        if(!isInLoopThread()) abortNotInLoopThread();
    }

    bool isInLoopThread() const
    {
        return threadId_ == muduo::CurrentThread::tid();
    }

    static EventLoop* getEventLoopOfCurrentThread();

    void wakeup();

    void runInLoop(const Functor& cb);
    void queueInLoop(const Functor& cb);
private:

    void abortNotInLoopThread();
    void handleRead(); // waked up
    void doPendingFunctors();

    using ChannelList = std::vector<Channel*>;

    bool looping_; // atomic
    bool quit_; // atomic
    bool callingPendingFunctors_; // atomic
    const pid_t threadId_;
    Timestamp pollReturnTime_;
    boost::scoped_ptr<Poller> poller_;
    boost::scoped_ptr<TimerQueue> timerQueue_;
    int wakeupFd_;
    boost::scoped_ptr<Channel> wakeupChannel_;
    ChannelList activeChannels_;
    MutexLock mutex_;
    std::vector<Functor> pendingFunctors_; // @GuardedBy mutex_;
};
} // namespace muduo

#endif

