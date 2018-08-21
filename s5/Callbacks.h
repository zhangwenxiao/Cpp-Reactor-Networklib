#ifndef __CALLBACK_H__
#define __CALLBACK_H__

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace muduo {
    class TcpConnection;

    using TcpConnectionPtr = boost::shared_ptr<TcpConnection>;

    using TimerCallback = boost::function<void()>;
    using ConnectionCallback = boost::function<void(const TcpConnectionPtr&)>;
    using MessageCallback = boost::function<void(const TcpConnectionPtr&, 
                             const char* data, 
                             ssize_t len)>;
}

#endif
