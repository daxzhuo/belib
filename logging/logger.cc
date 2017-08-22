#include "logger.h"
#include <stdio.h>
#include <sys/time.h>

namespace logging {

__thread time_t t_lastSecond;
__thread char t_time[32];

const int kMicroSecondsPerSecond = 1000 * 1000;

uint64_t gettid() {
  pthread_t tid = pthread_self();
  uint64_t thread_id = 0;
  memcpy(&thread_id, &tid, std::min(sizeof(thread_id), sizeof(tid)));
  return thread_id;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v) {
  s.append(v.data_, v.len_);
  return s;
}

void Logger::Impl::formatTime() {
  timeval tv;
  time_t seconds = tv.tv_sec;
  suseconds_t microSeconds = tv.tv_usec;
  if (seconds != t_lastSecond) {
    t_lastSecond = seconds;
    struct tm tm_time;
    ::localtime_r(&seconds, &tm_time);
    int len = snprintf(t_time, sizeof t_time, "%4d%02d%02d %02d:%02d:%02d",
      tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
      tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    assert(len == 17); (void)len;
  }
  ::snprintf(t_time+17, sizeof(t_time) - 17, ".%06d", microSeconds);
  stream_ << string(t_time, 17+7);
}

Logger::Impl::Impl(LogLevel level, const SourceFile& file, int line)
  : level_(level), basename_(file), line_(line) {
    formatTime();
    stream_ << gettid();
    stream_ << LogLevelName[level];
}

//Logger::Logger(LogLevel level, int savedErrno, SourceFile& file, int line)
//  : impl_(level, savedErrno, file, line) {
//}

Logger::Logger(LogLevel level, SourceFile file, int line) 
  : impl_(level, file, line) {
}

Logger::~Logger() {
  impl_.finish();
}

void Logger::Impl::finish() {
  stream_ << " - " << basename_ << ':' << line_ << '\n';
  const LogStream::Buffer& buf(stream_.buffer());
  g_output(buf.data(), buf.length());
  if (level_ == LOGLEVEL_FATAL)
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
