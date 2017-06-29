
#include <sys/epoll.h>
#include <assert.h>
#include <string.h>
#include "epoll.h"
#include "channel.h"

namespace network {

Epoll::Epoll(EventLoop* el) 
  : Poller(el), epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
    events_(kInitEventListSize) {}

void Epoll::update(int operation, Channel* ch) {
	struct epoll_event event;
	bzero(&event, sizeof(event));
	event.events = ch->Events();
	event.data.ptr = ch;
	int fd = ch->Fd();
	if (::epoll_ctl(epollfd_, operation, fd, &event) < 0) {
		perror("epoll_ctl error");
	}
}

void Epoll::AddChannel(Channel* ch) {
	int fd = ch->Fd();
	assert(channels_.find(fd) == channels_.end());
	channels_[fd] = ch;
	if (channels_.size() >= events_.size()) {
		events_.resize(events_.size()*2);
	}
	update(EPOLL_CTL_ADD, ch);
}

void Epoll::RemoveChannel(Channel* ch) {
	int fd = ch->Fd();
	assert(channels_.find(fd) != channels_.end());
	channels_.erase(fd);
	update(EPOLL_CTL_DEL, ch);
}

void Epoll::UpdateChannel(Channel* ch) {
	int fd = ch->Fd();
	assert(channels_.find(fd) != channels_.end());
	update(EPOLL_CTL_MOD, ch);
}

void Epoll::Poll(int ms, ActiveChannelList* aclist) {
	int numEvent = ::epoll_wait(epollfd_, events_.data(),
				 static_cast<int>(events_.size()), ms);
	int savedErrno = errno;
	if (numEvent > 0) {
		activateChannels(numEvent, aclist);
	}
}

void Epoll::activateChannels(int numEvent, ActiveChannelList* aclist) {
	for (int i = 0; i < numEvent; ++i) {
		Channel* ch = static_cast<Channel*>(events_[i].data.ptr);
		ch->SetEvent(events_[i].events);
		aclist->push_back(ch);
	}
}

}
