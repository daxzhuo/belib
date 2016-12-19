
#ifndef NETWORK_EVENT_LOOP_H_
#define NETWORK_EVENT_LOOP_H_

#include <vector>
#include <memory>

namespace network {
class Channel;
class Poller;

class EventLoop
{
public:
	EventLoop();


	void StartLoop();
	void StopLoop() { killed = true; }

	void AddChannel(Channel* ch);
	void RemoveChannel(Channel* ch);
	void UpdateChannel(Channel* ch);
private:
	typedef std::vector<Channel*> ActiveChannelList;
	// unique_ptr ?
	Poller* poller_;
	ActiveChannelList aclist_;
	bool killed;
};

}

#endif