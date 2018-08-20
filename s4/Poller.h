#ifndef __POLLER_H__
#define __POLLER_H__

#include "EventLoop.h"

#include <muduo/base/Timestamp.h>

#include <map>
#include <vector>

struct pollfd;

namespace muduo {
    // 前置声明，避免包含Channel类的头文件
    class Channel;
    class EventLoop;

    class Poller : boost::noncopyable {
    public:
        using ChannelList = std::vector<Channel*>;

        Poller(EventLoop* loop);
        ~Poller();

        // Polls the I/O events
        // Must be called in the loop thread
        Timestamp poll(int timeoutMs, ChannelList* activeChannels);

        // Changes the interested I/O events
        // Must be called in the loop thread
        void updateChannel(Channel* channel);

        void assertInLoopThread() { ownerLoop_ -> assertInLoopThread(); }

    private:
        void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;

        using PollFdList = std::vector<struct pollfd>;
        using ChannelMap = std::map<int, Channel*>;

        EventLoop* ownerLoop_;
        PollFdList pollfds_;
        ChannelMap channels_;
    };
}

#endif
