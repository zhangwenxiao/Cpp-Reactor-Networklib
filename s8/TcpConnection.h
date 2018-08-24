#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include "Buffer.h"
#include "Callbacks.h"
#include "InetAddress.h"

#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace muduo {

class Channel;
class EventLoop;
class Socket;

class TcpConnection : boost::noncopyable, 
    public boost::enable_shared_from_this<TcpConnection> {
public:
    TcpConnection(EventLoop* loop, 
                  const std::string& name, 
                  int sockfd, 
                  const InetAddress& localAddr, 
                  const InetAddress& peerAddr);

    ~TcpConnection();

    EventLoop* getLoop() const { return loop_; }
    const std::string& name() const { return name_; }
    const InetAddress& localAddress() { return localAddr_; }
    const InetAddress& peerAddress() { return peerAddr_; }
    bool connected() const { return state_ == kConnected; }

    void setConnectionCallback(const ConnectionCallback& cb)
    {
        connectionCallback_ = cb;
    }

    void setMessageCallback(const MessageCallback& cb)
    {
        messageCallback_ = cb;
    }

    // Internal use only
    void setCloseCallback(const CloseCallback& cb)
    {
        closeCallback_ = cb;
    }

    void connectDestroyed(); // Should be called only once

    // called when TcpServer accepts a new connection
    void connectEstablished(); // should be called only once

    // Thread safe
    void send(const std::string& message);
    // Thread safe
    void shutdown();

private:
    enum StateE { kConnecting, kConnected, kDisconnecting, kDisconnected };

    void setState(StateE s) { state_ = s; }
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();
    void sendInLoop(const std::string& message);
    void shutdownInLoop();

    EventLoop* loop_;
    std::string name_;
    StateE state_; // FIXME: use atomic variable
    // we don't expose those classes to client
    boost::scoped_ptr<Socket> socket_;
    boost::scoped_ptr<Channel> channel_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;
};

using TcpConnectionPtr = boost::shared_ptr<TcpConnection>;

}


#endif
