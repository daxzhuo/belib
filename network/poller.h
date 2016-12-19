
#ifndef NETWORK_POLLER_H_
#define NETWORK_POLLER_H_

#include <unordered_map>

namespace network {
class EventLoop;
class Channel;
class Poller {
public:
	typedef std::vector<Channel*> ActiveChannelList;
	Poller(EventLoop* el) : el_(el) {}
	virtual ~Poller() {}
	virtual void Poll(int ms, ActiveChannelList* aclist) = 0;
	virtual void UpdateChannel(Channel* ch) = 0;
	virtual void AddChannel(Channel* ch) = 0;
	virtual void RemoveChannel(Channel* ch)  = 0;
protected:

	typedef std::unordered_map<int, Channel*> ChannelMap;
	ChannelMap channels_;
private:
	EventLoop* el_;
};
}

#endif