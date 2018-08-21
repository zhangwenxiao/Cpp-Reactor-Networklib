#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include "Channel.h"
#include "Socket.h"

namespace muduo {

class EventLoop;
class InetAddress;

class Acceptor : boost::noncopyable {
public:

    using NewConnectionCallback = boost::function<void(int sockfd, const InetAddress&)>;

    Acceptor(EventLoop* loop, const InetAddress& listenAddr);

    void setNewConnectionCallback(const NewConnectionCallback& cb)
    {
        newConnectionCallback_ = cb;
    }

    bool listenning() const
    {
        return listenning_;
    }

    void listen();
private:

    void handleRead();

    EventLoop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listenning_;
}; // class Acceptor
} // namespace muduo

#endif
