#include "tcp_server.h"

namespace network {

Acceptor::Acceptor(EventLoop* loop, InetAddress& addr)
  : loop_(loop), acceptSocket(util::createNonblockingOrDie()),
    acceptChan_(loop, socket_.Fd()),
    listenning_(false) {
  acceptSocket_.setReuseAddr(true);
  acceptSocket_.bind(addr);
  acceptChan_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

void Acceptor::Listen() {
  listenning_ = true;
  acceptSocket_.listen();
  acceptChan_.enableReading();
}

void Acceptor::handleRead() {
  InetAddress addr;
  int connfd = acceptSocket_.accept(&addr);
  if (connfd > 0) {
    if (newConnCallback_) {
      newConnCallback_();
    } else {
      close();
    }
  }
}
  
} // network
