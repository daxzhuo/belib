#include "log_stream.h"

namespace logging {

namespace internal {

template class FixedBuffer<kSmallBuffer>;
template class FixedBuffer<kLargeBuffer>;
template <int SIZE> 
void FixedBuffer<SIZE>::cookieStart() {}

template <int SIZE> 
void FixedBuffer<SIZE>::cookieEnd() {}
} // internal
LogStream &LogStream::operator<<(uint64_t v) {
  char buffer[20];
  sprintf(buffer, "%uld", v);
  operator<<(buffer);
}

} // logging
