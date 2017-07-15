

#ifndef NETWORK_EPOLL_H_
#define NETWORK_EPOLL_H_

#include <vector>
#include "poller.h"

namespace network {

class Epoll : public Poller {
public:
	Epoll(EventLoop* el);
	
	virtual void Poll(int ms, ActiveChannelList* aclist) override;
	virtual void UpdateChannel(Channel* ch) override;
	virtual void AddChannel(Channel* ch) override;
	virtual void RemoveChannel(Channel* ch) override;
	
	
private:
	
	void activateChannels(int numEvent, ActiveChannelList* aclist);
	void update(int operation, Channel* ch);
	typedef std::vector<struct epoll_event> EventList;
    int epollfd_;
	EventList events_;
	
	static const int kInitEventListSize = 16;
};

}

#endif
