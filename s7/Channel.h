#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <muduo/base/Timestamp.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace muduo {

class EventLoop;

class Channel : boost::noncopyable {
public:
    using EventCallback = boost::function<void()>;
    using ReadEventCallback = boost::function<void(Timestamp)>;

    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handleEvent(Timestamp receiveTime);

    void setReadCallback(const ReadEventCallback& cb) 
    { 
        readCallback_ = cb; 
    }

    void setWriteCallback(const EventCallback& cb) 
    { 
        writeCallback_ = cb; 
    }

    void setErrorCallback(const EventCallback& cb) 
    { 
        errorCallback_ = cb; 
    }

    void setCloseCallback(const EventCallback& cb)
    {
        closeCallback_ = cb;
    }

    int fd() const { return fd_; }
    int events() const { return events_; }
    void set_revents(int revt) { revents_ = revt; }
    bool isNoneEvent() const { return events_ == kNoneEvent; }

    void enableReading() 
    { 
        events_ |= kReadEvent; 
        update(); 
    }

    void disableAll()
    {
        events_ = kNoneEvent;
        update();
    }

    int index() { return index_; }
    void set_index(int idx) { index_ = idx; }

    EventLoop* ownerLoop() { return loop_; }

private:
    void update();

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop* loop_;
    const int fd_;
    int events_;
    int revents_;
    int index_;

    bool eventHandling_;

    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
    EventCallback closeCallback_;
    }; // class Channel
} // namespace muduo

#endif
