#ifndef _LOG_STREAM_H_
#define _LOG_STREAM_H_

#include <ext/vstring.h>
#include <ext/vstring_fwd.h>
#include <functional>
#include <string.h>
#include <string>
namespace logging {

typedef __gnu_cxx::__sso_string string;
const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;
namespace internal {
// TODO: make the buffer adjustable
/*
 * Solution 1: use one fixed buffer or dynamic allocate(not enough space)
 * Solution 2: use both, maybe use writev()
 */
template <int SIZE> 
class FixedBuffer {
public:
  FixedBuffer() : cur_(data_) { setCookieFunc(cookieStart); }

  ~FixedBuffer() { setCookieFunc(cookieEnd); }

  void append(const char *buf, size_t len) {
    if (avail() > len) {
      memcpy(cur_, buf, len);
      cur_ += len;
    }
  }

  size_t avail() { return end() - cur_; }
  const char *data() const { return data_; }
  void bzero() { ::bzero(data_, length()); }
  int length() const { return static_cast<int>(cur_ - data_); }
  void reset() { cur_ = data_; }

private:
  const char *end() const { return data_ + SIZE; }
  static void cookieStart();
  static void cookieEnd();

  typedef std::function<void()> cookieFunc;

  void setCookieFunc(cookieFunc cookie) { cookie_ = cookie; }
  cookieFunc cookie_;
  char data_[SIZE];
  char *cur_;
};
} // internal

class LogStream {
public:
  typedef internal::FixedBuffer<kSmallBuffer> Buffer;
  LogStream() {}
  ~LogStream() {}
  LogStream &operator<<(const string &v) {
    buffer_.append(v.c_str(), v.size());
    return *this;
  }
  LogStream &operator<<(const std::string &v) {
    buffer_.append(v.data(), v.size());
    return *this;
  }
  // LogStream& operator<<(StringPiece& v);
  LogStream &operator<<(uint64_t);
  LogStream &operator<<(const char *str) {
    if (str) {
      buffer_.append(str, strlen(str));
    } else {
      buffer_.append("(null)", 6);
    }
    return *this;
  }
  const Buffer &buffer() { return buffer_; }
  void append(const char *data, int len) { buffer_.append(data, len); }

private:
  Buffer buffer_;
};
} // logging

#endif
