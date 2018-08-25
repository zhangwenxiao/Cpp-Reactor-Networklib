#include "Channel.h"
#include "EventLoop.h"

#include <muduo/base/Logging.h>
#include <muduo/base/Timestamp.h>

#include <sstream>
#include <poll.h>

using namespace muduo;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI; // 有数据可读 | 有紧迫数据可读
const int Channel::kWriteEvent = POLLOUT; // 写数据不会导致阻塞

Channel::Channel(EventLoop* loop, int fdArg)
    : loop_(loop),
      fd_(fdArg),
      events_(0),
      revents_(0),
      index_(-1),
      eventHandling_(false) {}

void Channel::update()
{
    loop_ -> updateChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
    eventHandling_ = true;

    // 指定的文件描述符非法
    if(revents_ & POLLNVAL) {
        LOG_WARN << "Channel::handle_event() POLLNVAL";
    }

    if((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
        LOG_WARN << "Channel::handle_event() POLLHUP";
        if(closeCallback_) closeCallback_();
    }

    // 指定的文件描述符发生错误
    if(revents_ & (POLLERR | POLLNVAL)) {
        if(errorCallback_) errorCallback_();
    }

    // POLLRDHUP 流套接字的远程端断开连接，或者关闭了写端
    if(revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if(readCallback_) readCallback_(receiveTime);
    }

    if(revents_ & POLLOUT) {
        if(writeCallback_) writeCallback_();
    }

    eventHandling_ = false;
}

Channel::~Channel()
{
    assert(!eventHandling_);
}

