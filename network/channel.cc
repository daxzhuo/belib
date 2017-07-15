#include "channel.h"
#include <stdio.h>
#include "event_loop.h"
#include <sys/epoll.h>
namespace network {

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

void Channel::update()  { el_->UpdateChannel(this); }

Channel::Channel(EventLoop* el, int fd)
    : el_(el), fd_(fd), events_(0), revents_(0) { Register(); }

Channel::~Channel() {
	el_->RemoveChannel(this);
}

void Channel::Register() {
	el_->AddChannel(this);
}

void Channel::HandleEvent() {
	
	if (revents_ & EPOLLERR) {
		printf("Epoll error");
	}
	if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
		if (rcb_) rcb_();
	}
	if (revents_ & EPOLLOUT) {
		if (wcb_) wcb_();
	}
}



}
