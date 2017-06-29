#include "logger.h"

namespace logging {

Logger::Impl::Impl(LogLevel level, SourceFile& file, int line)
  : level_(level), basename_(file), line_(line) {
    formatTime();
    stream_ << gettid();
    stream_ << level_;
}

Logger::Logger(LogLevel level, SourceFile file, int line) 
  : impl_(LogLevel level, SourceFile file, int line) {
  
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




void Logger::Impl::

} // logging