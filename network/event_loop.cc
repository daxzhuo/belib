#include <assert.h>
#include <sys/eventfd.h>
#include "event_loop.h"
#include "channel.h"
#include "epoll.h"

namespace network {

const int kPollTimeMs = 10000;

namespace {
int createWakeupFd() {
	int evfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evfd < 0) {
		LOG
	}
	return evfd;
}
}

EventLoop::EventLoop() : poller_(new Epoll(this)),
		wakeupFd_(createWakeupFd()),
		stopped_(true) {}
EventLoop::~EventLoop() { delete poller_; }
void EventLoop::AddChannel(Channel* ch) { poller_->AddChannel(ch); }
void EventLoop::RemoveChannel(Channel* ch) { poller_->RemoveChannel(ch); }
void EventLoop::UpdateChannel(Channel* ch) { poller_->UpdateChannel(ch); }


void EventLoop::Start() {
	stopped_ = false;

	while (!stopped_) {
		aclist_.clear();
		poller_->Poll(kPollTimeMs, &aclist_);
		for (auto it = aclist_.begin(); it != aclist_.end(); ++it) {
			(*it)->HandleEvent();
		}
	}
}

void EventLoop::Stop() {
	stopped_ = true;
	if (isInLoop)
	wakeup();
}

}
