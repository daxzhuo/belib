
#include <functional>
#include "event_loop.h"
class Acceptor {
public:
	typedef std::function<void (int fd, const InetAddress&)> NewConnCb;
	Acceptor(EventLoop* el, const InetAddress& addr);
	void setNewConnCb(const NewConnCb& cb) {
		cb_ = cb;
	}
private:
	NewConnCb cb_;
};

// one listening port
struct TcpListener {
	int fd;
	
};
