#include "logger.h"

namespace logging {

__thread time_t t_lastSecond;

uint64_t gettid() {
  pthread_t tid = pthread_self();
  uint64_t thread_id = 0;
  memcpy(&thread_id, &tid, std::min(sizeof(thread_id), sizeof(tid)));
  return thread_id;
}

Logger::Impl::formatTime() {
  int64_t microSecondsSinceEpoch = time_.microSecondsSinceEpoch();
  time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / Timestamp::kMicroSecondsPerSecond);
  int64_t microSeconds = static_cast<int>(microSecondsSinceEpoch % Timestamp::kMicroSecondsPerSecond);
  if (seconds != t_lastSecond) {
    t_lastSecond = seconds;
    struct tm tm_time;
    if (g_timeZone.valid()) {
      tm_time = g_timeZone.toLocalTime(seconds);
    } else {
      ::gmtime_r(&seconds, &tm_time);
    }
    int len = snprintf(t_time, sizeof t_time, "%4d%02d%02d %02d:%02d:%02d",
      tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
      tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    assert(len == 17); (void)len;
  }
  snprintf(t_time+17, sizeof(t_time) - 17, ".%06d", microSeconds);
  // TODO: find out what 17 mean
  stream_ << T(t_time, 17);
}

Logger::Impl::Impl(LogLevel level, SourceFile& file, int line)
  : level_(level), basename_(file), line_(line) {
    formatTime();
    stream_ << gettid();
    stream_ << level_;
}

Logger::Logger(LogLevel level, int savedErrno, SourceFile file, int line) 
  : impl_(LogLevel level, savedErrno, SourceFile file, int line) {
}

Logger::Logger(LogLevel level, SourceFile file, int line) 
  : impl_(LogLevel level, 0, SourceFile file, int line) {
}

Logger::~Logger() {
  impl_.finish();
}

void Logger::Impl::finish() {
  stream_ << " - " << basename_ << ':' << line_ << '\n';
  g_output(stream_.data(), stream.len_);
  if (impl_.level_ == LOGLEVEL_FATAL)
  {
    g_flush();
    abort();
  }
}


void defaultOutput(const char* msg, int len)
{
  fwrite(msg, 1, len, stdout);
}

void defaultFlush()
{
  fflush(stdout);
}

} // logging
