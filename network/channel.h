
#ifndef NETWORK_CHANNEL_H_
#define NETWORK_CHANNEL_H_

#include <functional>

namespace network {
class EventLoop;
class Channel {
 public:
  typedef std::function<void()> Closure;
  Channel(EventLoop* el, int fd);
  ~Channel();
  void ReadCallback(const Closure& cb) { rcb_ = cb; }
  void WriteCallback(const Closure& cb) { wcb_ = cb; }
  void HandleEvent();
  void Register();

  void SetEvent(int revent) { revents_ = revent; }
  int Events() { return events_; }
  int Fd() { return fd_; }
  void EnableReading() { events_ |= kReadEvent; update(); }
  void EnableWriting() { events_ |= kWriteEvent; update(); }
  void DisableWriting() { events_ &= ~kWriteEvent; update(); }
  void DisableAll() { events_ = kNoneEvent; update(); }
 private:
  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

  void update();
  EventLoop* el_;
  int fd_;
  Closure rcb_;
  Closure wcb_;
  short events_;
  short revents_;
};

}

#endif
