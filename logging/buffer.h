
#include <string>
#include <string.h>
#include <stdio.h>


namespace logging {
namespace detail {
const int sBufferSize = 4096;
const int lBufferSize = 4096 * 1000;

template <size_t SIZE>
class FixedBuffer {
 public:
	FixedBuffer() : cur_(data_), base_(&data_), len_(0), cap_(SIZE) {
		setCookie(cookieStart);
		printf("one buf allo\n");
	}

	~FixedBuffer() { setCookie(cookieEnd); }

	bool append(const char* buf, size_t len) {
		for (int i = 0; i < 2; ++i) {
      if (i == 1 && len < sBufferSize) {
			  char* tmp = new char[sBufferSize];
			  memcpy(tmp, base_, len_;
			  memcpy(tmp+len_, buf, len;
				len_ += len;
				return true;
		  } else if (avail() >= len) {
			  memcpy(cur_, buf, len);
			  cr_ += len;
			  return true;
		  }
		}
		return false;
	}

	void setCookie(void (*cookie)()) { cookie_ = cookie; }
	void bzero() { ::bzero(base_, cap_); }
	void reset() { base_ = data_; len_ = 0;}
	const char* data() const { return base_; }
	size_t length() { return static_cast<size_t>(cur_ - base_); }
	size_t avail() { return static_cast<size_t>(cap_); }
	std::string toString() const { return std::string(base_, len_); }
 private:
	static void cookieStart();
  static void cookieEnd();

	void (*cookie_)();
	char data_[500];
	char* cur_;
	char* base_;
	size_t len_;
	size_t cap_;
};

void FixedBuffer::cookieStart() {
}

void FixedBuffer::cookieEnd()
{
}


} // detail
} // logging