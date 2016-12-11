
#include <string>
#include <string.h>
#include <stdio.h>


namespace logging {
namespace detail {
const int sbuffersize = 4096;
const int lbuffersize = 4096 * 1000;

template <size_t SIZE>
class FixedBuffer {
public:
	FixedBuffer() : cur_(data_) {
		setCookie(cookieStart);
		printf("one buf allo\n");
	}

	~FixedBuffer() { setCookie(cookieEnd); }

	bool append(const char* buf, size_t len) {
		if (avail() >= len) {
			memcpy(cur_, buf, len);
			cur_ += len;
			return true;
		}
		return false;
	}

	void setCookie(void (*cookie)()) { cookie_ = cookie; }
	void bzero() { ::bzero(data_, SIZE); }
	void reset() { cur_ = data_; }
	const char* data() const { return data_; }
	size_t length() { return static_cast<size_t>(cur_-data_); }
	size_t size() { return SIZE; }
	size_t avail() { return static_cast<size_t>(data_ + SIZE - cur_); }
	std::string toString() const { return std::string(data_, cur_ - data_); }
private:
	static void cookieStart();
  static void cookieEnd();

	void (*cookie_)();
	char data_[SIZE];
	char* cur_;
};

template <size_t SIZE>
void FixedBuffer<SIZE>::cookieStart()
{
}

template <size_t SIZE>
void FixedBuffer<SIZE>::cookieEnd()
{
}


} // detail
} // logging