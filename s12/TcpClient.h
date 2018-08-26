#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include "TcpConnection.h"
#include <muduo/base/Mutex.h>
#include <boost/noncopyable.hpp>

namespace muduo {
class Connector;

using ConnectorPtr = boost::shared_ptr<Connector>;

class TcpClient : boost::noncopyable {
public:
    TcpClient(EventLoop* loop, const InetAddress& serverAddr);
    ~TcpClient();

    void connect();
    void disconnect();
    void stop();

    TcpConnectionPtr connection() const
    {
        MutexLockGuard lock(mutex_);
        return connection_;
    }

    bool retry() const;
    void enableRetry() { retry_ = true; }

    void setConnectionCallback(const ConnectionCallback& cb)
    {
        connectionCallback_ = cb;
    }

    void setMessageCallback(const MessageCallback& cb)
    {
        messageCallback_ = cb;
    }

    void setWriteCompleteCallback(const WriteCompleteCallback& cb)
    {
        writeCompleteCallback_ = cb;
    }

private:
    // Not thread safe, but in loop
    void newConnection(int sockfd);
    // Not thread safe, but in loop
    void removeConnection(const TcpConnectionPtr& conn);

    EventLoop* loop_;
    ConnectorPtr connector_; // Avoid revealing Connector
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    bool retry_; // atomic
    bool connect_; // atomic

    // Always in loop thread
    int nextConnId_;
    mutable MutexLock mutex_;
    TcpConnectionPtr connection_;
}; // class TcpClient

} // namespace muduo

#endif
