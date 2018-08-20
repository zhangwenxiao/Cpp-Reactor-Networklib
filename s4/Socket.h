#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <boost/noncopyable.hpp>

namespace muduo {
class InetAddress;

class Socket : boost::noncopyable {
public:

    explicit Socket(int sockfd) : sockfd_(sockfd) {}

    ~Socket();

    int fd() const { return sockfd_; }

    void bindAddress(const InetAddress& localaddr);

    void listen();

    int accept(InetAddress* peeraddr);

    void setReuseAddr(bool on);
private:

    const int sockfd_;
}; // class Socket
} // namespace muduo

#endif
