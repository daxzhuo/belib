

#include <sys/timerfd.h>
#include <string.h>
#include <unistd.h>
#include "event_loop.h"
#include "channel.h"
using namespace network;
EventLoop* g_loop;

void timeout() {
	printf("time out!\n");
	g_loop->StopLoop();
}

int main() {
	EventLoop loop;
	g_loop = &loop;
	int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	Channel channel(&loop, timerfd);
	channel.ReadCallback(timeout);
	channel.EnableReading();

	struct itimerspec howlong;
	bzero(&howlong, sizeof(howlong));
	howlong.it_value.tv_sec = 5;
	::timerfd_settime(timerfd, 0, &howlong, NULL);

	loop.StartLoop();

	::close(timerfd);
}