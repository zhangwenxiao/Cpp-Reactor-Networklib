#include "Channel.h"
#include "EventLoop.h"
#include "Poller.h"
#include "TimerId.h"
#include "TimerQueue.h"

#include <muduo/base/Logging.h>

#include <assert.h>

using namespace muduo;

__thread EventLoop* t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

EventLoop::EventLoop() 
    : looping_(false), 
      quit_(false), 
      threadId_(muduo::CurrentThread::tid()), 
      poller_(new Poller(this)),
      timerQueue_(new TimerQueue(this))
{
    LOG_TRACE << "EventLoop created " << this << " in thread " << threadId_;
    if(t_loopInThisThread) {
        LOG_FATAL << "Another EventLoop " << t_loopInThisThread 
            << " exists in this thread " << threadId_;
    } else {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    assert(!looping_);
    t_loopInThisThread = NULL;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

void EventLoop::loop()
{
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;

    while(!quit_) {
        activeChannels_.clear();
        pollReturnTime_ = poller_ -> poll(kPollTimeMs, &activeChannels_);
        for(auto it = activeChannels_.begin();
            it != activeChannels_.end(); ++it) {
            (*it) -> handleEvent();
        }
    }

    LOG_TRACE << "EventLoop " << this << " stop looping";
    looping_ = false;
}

void EventLoop::quit()
{
    quit_ = true;
}

void EventLoop::updateChannel(Channel* channel)
{
    assert(channel -> ownerLoop() == this);
    assertInLoopThread();
    poller_ -> updateChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
              << " was created in threadId_ = " << threadId_
              << ", current thread id = " << muduo::CurrentThread::tid();
}

TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
{
    return timerQueue_ -> addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback& cb)
{
    Timestamp time(addTime(Timestamp::now(), delay));
    return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback& cb)
{
    Timestamp time(addTime(Timestamp::now(), interval));
    return timerQueue_ -> addTimer(cb, time, interval);
}

