#ifndef __TIMER_QUEUE_H__
#define __TIMER_QUEUE_H__

#include "Callbacks.h"
#include "Channel.h"
#include "TimerId.h"

#include <boost/noncopyable.hpp>
#include <muduo/base/Timestamp.h>

#include <set>
#include <vector>

namespace muduo {
class EventLoop;
class Timer;
// A best efforts timer queue
// No guarantee that the callback will be on time
class TimerQueue : boost::noncopyable {
public:
    TimerQueue(EventLoop* loop);
    ~TimerQueue();

    // Schedules the callback to be run at given time
    // repeats if @c interval > 0.0
    // Must be thread safe
    // Usually be called from other threads
    TimerId addTimer(const TimerCallback& cb, 
                     Timestamp when, double interval);

private:
    // FIXME: use unique_ptr<Timer> instead of raw pointers
    using Entry = std::pair<Timestamp, Timer*>;
    using TimerList = std::set<Entry>;

    // called when timerfd alarms
    void handleRead();
    // move out all expired timers
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry>& expired, Timestamp now);

    bool insert(Timer* timer);

    EventLoop* loop_;
    const int timerfd_;
    Channel timerfdChannel_;
    // Timer list sorted by expiration
    TimerList timers_;
};
} // namespace muduo

#endif
