
#include "timestamp.h"
#include <sys/time.h>

using namespace base;

Timestamp Timestamp::now() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int64_t seconds = tv.tv_sec;
  return Timestamp(seconds * Timestamp::kMicroSecondsPerSecond + tv.tv_usec);
}