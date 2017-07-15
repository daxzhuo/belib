
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
	~EventLoop();

	void Start();
	void Stop();

	void AddChannel(Channel* ch);
	void RemoveChannel(Channel* ch);
	void UpdateChannel(Channel* ch);
private:
	typedef std::vector<Channel*> ActiveChannelList;
	const pid_t threadId_;
	std::unique_ptr<Poller> poller_;
	ActiveChannelList aclist_;
	int wakeupFd_;
	bool stopped_;
};

}

#endif
