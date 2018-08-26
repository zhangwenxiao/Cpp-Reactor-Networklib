#ifndef __TIMER_ID_H__
#define __TIMER_ID_H__

#include <muduo/base/copyable.h>

namespace muduo {
    class Timer;

    class TimerId : public muduo::copyable {
    public:
        explicit TimerId(Timer* timer = NULL, int64_t seq = 0) 
            : timer_(timer), sequence_(seq) {}

        friend class TimerQueue;

    private:
        Timer* timer_;
        int64_t sequence_;
    };
}

#endif
