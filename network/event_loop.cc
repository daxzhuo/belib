
#include "event_loop.h"
#include "channel.h"
#include "epoll.h"
namespace network {

const int kPollTimeMs = 10000;


EventLoop::EventLoop() : poller_(new Epoll(this)) {}

void EventLoop::AddChannel(Channel* ch) { poller_->AddChannel(ch); }
void EventLoop::RemoveChannel(Channel* ch) { poller_->RemoveChannel(ch); }
void EventLoop::UpdateChannel(Channel* ch) { poller_->UpdateChannel(ch); }


void EventLoop::StartLoop() {
	killed = false;

	while (!killed) {
		aclist_.clear();
		poller_->Poll(kPollTimeMs, &aclist_);
		for (auto it = aclist_.begin(); it != aclist_.end(); ++it) {
			(*it)->HandleEvent();
		}
	}
}

}