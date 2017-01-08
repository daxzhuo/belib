
#include "tcp_socket.h"

namespace network {

class EndPoint;

class TCPSocket {
public:
  int Bind(const EndPoint& address);
  int Listen(int backlog);
  int Accept(std::unique_ptr<TCPSocket>* socket,
             EndPoint* address,
             const CompletionCallback& callback);
  bool IsConnected() const;
  int Read(Buffer* buf, int buf_len, const CompletionCallback);
  int Write(Buffer* buf, int buf_len, const CompletionCallback);

  int SetAddressReuse(bool allow);
  int SetKeepAlive(bool enable, int delay);
  int SetNoDelay(bool no_delay);

  void Close();

private:
  int fd_;
};

} // network
