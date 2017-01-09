#include <string>

namespace network {
class EndPoint {
public:
  EndPoint(std::string address, uint16_t port);
private:
  struct sockaddr_in addr_;
};

}// network
