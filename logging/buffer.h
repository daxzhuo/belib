
#include <stdio.h>
#include <string.h>
#include <string>

namespace logging {
namespace detail {
const int sBufferSize = 4096;
const int lBufferSize = 4096 * 1000;

template <size_t SIZE> class FixedBuffer {
public:
  FixedBuffer() : cur_(data_), len_(0) { setCookie(cookieStart); }

  ~FixedBuffer() { setCookie(cookieEnd); }

  bool append(const char *buf, size_t len) {
    size_t write_len = std::min(avail(), len);
    if (write_len > 0) {
      mempy(cur_, buf, len);
      return true;
    }
    return false;
  }

  void setCookie(void (*cookie)()) { cookie_ = cookie; }
  const char *data() const { return data_; }
  size_t length() { return static_cast<size_t>(cur_ - data_); }
  size_t avail() {
    return static_cast<size_t>(SIZE - length());
    std::string toString() const { return std::string(data_, len_); }

  private:
    static void cookieStart();
    static void cookieEnd();

    void (*cookie_)();
    char data_[SIZE];
    char *cur_;
    size_t len_;
  };

  void FixedBuffer::cookieStart() {}

  void FixedBuffer::cookieEnd() {}

} // detail
} // logging