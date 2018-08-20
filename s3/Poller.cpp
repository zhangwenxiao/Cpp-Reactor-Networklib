#include "Channel.h"
#include "Poller.h"

#include <muduo/base/Logging.h>

#include <assert.h>
#include <poll.h>

using namespace muduo;

Poller::Poller(EventLoop* loop) : ownerLoop_(loop) {}

Poller::~Poller() {}

Timestamp Poller::poll(int timeoutMs, ChannelList* activeChannels)
{
    // XXX pollfds_ shouldn't change
    // pollfds_.data() 返回vector首元素地址
    int numEvents = ::poll(pollfds_.data(), pollfds_.size(), timeoutMs);
    Timestamp now(Timestamp::now());
    if(numEvents > 0) {
        LOG_TRACE << numEvents << " events happened";
        fillActiveChannels(numEvents, activeChannels);
    } else if(numEvents == 0) {
        LOG_TRACE << " nothing happened";
    } else {
        LOG_SYSERR << "Poller::poll()";
    }

    return now;
}

void Poller::fillActiveChannels(int numEvents, 
                                ChannelList* activeChannels) const
{
    for(PollFdList::const_iterator pfd = pollfds_.begin(); 
        pfd != pollfds_.end() && numEvents > 0; ++pfd) {
        if(pfd -> revents > 0) {
            --numEvents;
            ChannelMap::const_iterator ch = channels_.find(pfd -> fd);
            assert(ch != channels_.end());
            Channel* channel = ch -> second;
            assert(channel -> fd() == pfd -> fd);
            channel -> set_revents(pfd -> revents);
            activeChannels -> push_back(channel);
        }
    }
}

void Poller::updateChannel(Channel* channel)
{
    assertInLoopThread();

    LOG_TRACE << "fd = " << channel -> fd() 
              << " events = " << channel -> events();

    if(channel -> index() < 0) {
        // a new one, add to pollfds_
        assert(channels_.find(channel -> fd()) == channels_.end());

        struct pollfd pfd;
        pfd.fd = channel -> fd();
        pfd.events = static_cast<short>(channel -> events());
        pfd.revents = 0;
        pollfds_.push_back(pfd);

        int idx = static_cast<int>(pollfds_.size()) - 1;
        channel -> set_index(idx);

        channels_[pfd.fd] = channel;
    } else {
        // update existing one
        assert(channels_.find(channel -> fd()) != channels_.end());
        assert(channels_[channel -> fd()] == channel);

        int idx = channel -> index();
        assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));

        struct pollfd& pfd = pollfds_[idx];
        assert(pfd.fd == channel -> fd() || pfd.fd == -1);
        pfd.events = static_cast<short>(channel -> events());
        pfd.revents = 0;

        if(channel -> isNoneEvent()) {
            // ignore this pollfd
            pfd.fd = -1;
        }
    }
}
