#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "Callbacks.h"
#include "TcpConnection.h"

#include <map>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace muduo {

class Acceptor;
class EventLoop;

class TcpServer : boost::noncopyable {
public:
 
    TcpServer(EventLoop* loop, const InetAddress& listenAddr);
    ~TcpServer(); // force out-line dtor, for scoped_ptr members

    // Start the server if it's not listenning
    // It's harmless to call it multiple times
    // Thread safe
    void start();

    // Set connection callback
    // Not thread safe
    void setConnectionCallback(const ConnectionCallback& cb)
    {
        connectionCallback_ = cb;
    }

    // Set message callback
    // Not thread safe
    void setMessageCallback(const MessageCallback& cb)
    {
        messageCallback_ = cb;
    }
private:
    // Not thread safe, but in loop
    void newConnection(int sockfd, const InetAddress& peerAddr);

    using ConnectionMap = std::map<std::string, TcpConnectionPtr>;

    EventLoop* loop_;
    const std::string name_;
    boost::scoped_ptr<Acceptor> acceptor_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    bool started_;
    int nextConnId_; // Always in loop thread
    ConnectionMap connections_;

}; // class TcpSerer

} // namespace muduo

#endif
