#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

#include <muduo/base/Thread.h>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <vector>

namespace muduo {
    class Channel;
    class Poller;

    class EventLoop : boost::noncopyable {
    public:

        EventLoop();
        ~EventLoop();

        void loop();
        void updateChannel(Channel* channel);
        void quit();
        
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

        using ChannelList = std::vector<Channel*>;

        bool looping_; // atomic
        bool quit_; // atomic
        const pid_t threadId_;
        boost::scoped_ptr<Poller> poller_;
        ChannelList activeChannels_;
    };
} // namespace muduo

#endif

