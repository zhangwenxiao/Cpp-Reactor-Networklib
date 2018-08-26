#ifndef __TIMER_H__
#define __TIMER_H__

#include "Callbacks.h"
#include <muduo/base/Timestamp.h>
#include <muduo/base/Atomic.h>
#include <boost/noncopyable.hpp>

namespace muduo {
    class Timer : boost::noncopyable {
    public:
        Timer(const TimerCallback& cb, Timestamp when, double interval)
            : callback_(cb),
              expiration_(when),
              interval_(interval),
              repeat_(interval > 0.0),
              sequence_(s_numCreated_.incrementAndGet()) {}
        
        void run() const { callback_(); }

        Timestamp expiration() const { return expiration_; }
        bool repeat() const { return repeat_; }
        int64_t sequence() const { return sequence_; }
        void restart(Timestamp now);
        
    private:
        const TimerCallback callback_;
        Timestamp expiration_;
        const double interval_;
        const bool repeat_;
        const int64_t sequence_;

        static AtomicInt64 s_numCreated_;
    };
}

#endif
