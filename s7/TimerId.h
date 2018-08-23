#ifndef __TIMER_ID_H__
#define __TIMER_ID_H__

#include <muduo/base/copyable.h>

namespace muduo {
    class Timer;

    class TimerId : public muduo::copyable {
    public:
        explicit TimerId(Timer* timer) : value_(timer) {}

    private:
        Timer* value_;
    };
}

#endif
