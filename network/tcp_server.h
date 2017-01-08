#include <functional>

namespace {

class Acceptor {
public:
  typedef std::function<void(int sockfd, InetAddress& addr)> NewConnCb;
  Acceptor(EventLoop* loop, const InetAddress listenAddr);
  void setNewConnCb(const NewConnCb& cb) { newConnCb_ = cb; }
  bool listenning() { return listenning_; }
  void Listen();
private:
  void handleRead();

  EventLoop* loop_;
  TCPSocket socket_;
  Channel acceptChan_;
  NewConnCb newConnCb_;
  bool listenning_;
};

class TcpServer {
public:
private:
  void OnAcceptCallback
};

}
