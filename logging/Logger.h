#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <algorithm>
#include <functional>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>


namespace logging {

void defaultOutput(const char* msg, int len)
{
  fwrite(msg, 1, len, stdout);
}

void defaultFlush()
{
  fflush(stdout);
}

uint64_t gettid() {
  pthread_t tid =  pthread_self();
  uint64_t thread_id = 0;
  memcpy(&thread_id, &tid, std::min(sizeof(thread_id), sizeof(tid)));
  return thread_id;
}

class Logger {
public:

  typedef std::function<void(const char*,int)> OutputFunc;
  typedef std::function<void()> FlushFunc;

  enum LogLevel {
    LOGLEVEL_TRACE,
    LOGLEVEL_DEBUG,
    LOGLEVEL_INFO,
    LOGLEVEL_WARN,
    LOGLEVEL_ERROR,
    LOGLEVEL_FATAL,
    LOGLEVEL_NUM_LOG_LEVELS,
  };

  static constexpr const char* LogLevelName[NUM_LOG_LEVELS] =
  {
  "TRACE",
  "DEBUG",
  "INFO ",
  "WARN ",
  "ERROR",
  "FATAL",
  };

  class SourceFile {
   public:
    template <int N>
    inline SourceFile(const char (&arr)[N]) 
      : data_(arr), len_(N-1) {
      const char* pslash = strrchr('/');
      if (pslash) {
        data_ = pslash + 1;
        len_ = static_cast<int>(data_ - arr);
      }
    }

    explicit SourceFile(const char* filename) : data_(filename) {
      const char* pslash = strrchr('/');
      if (pslash) {
        data_ = pslash + 1;
      }
      len_ = static_cast<int>(strlen(data_));
    }
   private:
    const char* data_;
    int len_;
  };

  Logger(LogLevel level, SourceFile file, int line);
  ~Logger();

  StringStream& stream() { return stream_; }

  void Logv(const char* format, va_list ap);

  static void setLogLevel(LogLevel level) {
    g_LogLevel = level;
  }

  static LogLevel logLevel() {
    return g_LogLevel;
  }
  // global level
  static LogLevel g_LogLevel;

  static OutputFunc g_output;
  static FlushFunc g_flush;

  typedef void (*OutputFunc)(const char* msg, int len);
  typedef void (*FlushFunc)();
  static void setOutput(OutputFunc);
  static void setFlush(FlushFunc);

private:
  LogLevel level_;
  SourceFile sourcefile_;
  int line_;
};

constexpr const char* Logger::LogLevelName[Logger::NUM_LOG_LEVELS];
Logger::LogLevel Logger::g_LogLevel;
Logger::OutputFunc Logger::g_output = defaultOutput;
Logger::FlushFunc Logger::g_flush = defaultFlush;

Logger::Logger(LogLevel level, const char *filename, int line) 
  : level_(level), filename_(filename), line_(line) {

    const char* pslash = strrchr(filename, '/');
    sourcefile_ = pslash ? pslash : filename;

}

void Logger::Logv(const char* format, va_list ap) {
  const uint64_t thread_id = gettid();
    for (int i = 0; i < 2; ++i) {
    char* base;
    int bufsize;
    if (i == 0) {
      bufsize = sizeof(buffer);
      base = buffer;
    } else {
      bufsize = 4096;
      base = new char[bufsize];
    }
    char* p = base;
    char* limit = base + bufsize;

    struct timeval now_tv;
    gettimeofday(&now_tv, NULL);
    const time_t seconds = now_tv.tv_sec;
    struct tm t;
    localtime_r(&seconds, &t);
    p += snprintf(p, limit - p,
                  "%04d/%02d/%02d-%02d:%02d:%02d.%06d %llx %s %s:%d ",
                  t.tm_year + 1900,
                  t.tm_mon + 1,
                  t.tm_mday,
                  t.tm_hour,
                  t.tm_min,
                  t.tm_sec,
                  static_cast<int>(now_tv.tv_usec),
                  static_cast<long long unsigned int>(thread_id), 
                  LogLevelName[level_],
                  sourcefile_,
                  line_);

    if (p < limit) {
      va_list backup_ap;
      va_copy(backup_ap, ap);
      p += vsnprintf(p, limit-p, format, backup_ap);
      va_end(backup_ap);
    }

    if (p >= limit) {
      if (i == 0) {
        continue;
      } else {
        p = limit-1;
      }
    }

    if (p == base || p[-1] != '\n') {
      *p++ = '\n';
    }

    assert(p <= limit);
    g_output(base, p - base);
    if (level_ == FATAL)
      g_flush();
    if (base != buffer) {
      delete[] base;
    }
    break;
  }

}

void Logger::setOutput(OutputFunc of) {
  g_output = of;
}

void Logger::setFlush(FlushFunc ff) {
  g_flush = ff;
}

} // logging



// TODO: make it well
#define LOGTRACE(...) do { if (logging::Logger::logLevel() <= logging::Logger::TRACE) \
logging::Logger(logging::Logger::TRACE, __FILE__, __LINE__, __VA_ARGS__); } while(0)
#define LOGDEBUG(...) do { if (logging::Logger::logLevel() <= logging::Logger::DEBUG)\
  logging::Logger(logging::Logger::DEBUG, __FILE__, __LINE__, __VA_ARGS__); } while (0)
#define LOGINFO(...) do { if (logging::Logger::logLevel() <= logging::Logger::INFO)\
logging::Logger(logging::Logger::INFO, __FILE__, __LINE__, __VA_ARGS__); } while(0)
#define LOGWARN(...) logging::Logger(logging::Logger::WARN, __FILE__, __LINE__, __VA_ARGS__)
#define LOGERROR(...) logging::Logger(logging::Logger::ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOGFATAL(...) logging::Logger(logging::Logger::FATAL, __FILE__, __LINE__, __VA_ARGS__)

#define LOG(LEVEL) logging::Logger(LEVEL)                       \
  ::logging::Logger(                                            \
    ::logging::LOGLEVEL_##LEVEL, __FILE__, __LINE__)

#endif
