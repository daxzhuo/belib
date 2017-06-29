
#include "tcp_socket.h"

namespace network {

class EndPoint;

class TCPSocket {
public:
  TCPSocket(int fd) : socket_(fd) {}
  TCPSocket(Socket&& s);
  int Bind(const EndPoint& address);
  int Listen(int backlog);
  int Accept(TCPSocket* socket,
             EndPoint* address);
  bool IsConnected() const;
  int Read(Buffer* buf, int buf_len);
  int Write(Buffer* buf, int buf_len);

  int SetAddressReuse(bool allow);
  int SetKeepAlive(bool enable, int delay);
  int SetNoDelay(bool no_delay);

  void Close();
  void ShutDown();

private:
  Socket socket_;
  // TODO: support fast open
};



} // network
