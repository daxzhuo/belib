#ifndef LOGGER_H_
#define LOGGER_H_
#include <algorithm>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
namespace logging {
	uint64_t defaultgettid() {
		pthread_t tid =  pthread_self();
		uint64_t thread_id = 0;
		memcpy(&thread_id, &tid, std::min(sizeof(thread_id), sizeof(tid)));
		return thread_id;
	}
	class Logger {
	private:
		FILE* file_;
		uint64_t (*gettid_)();
	public:
		Logger(FILE* file, uint64_t (*gettid)() = defaultgettid) : file_(file), gettid_(gettid) { }
		~Logger() {
			fclose(file_);
		}
		void Log(const char* format, ...) {
	    va_list ap;
	    va_start(ap, format);
	    Logv(format, ap);
	    va_end(ap);
		}
		virtual void Logv(const char* format, va_list ap) {
			const uint64_t thread_id = gettid_();
			char buffer[500];
			for (int i = 0; i < 2; ++i) {
				char* base;
				int bufsize;
				if (i == 0) {
	        bufsize = sizeof(buffer);
	        base = buffer;
	      } else {
	        bufsize = 30000;
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
	                    "%04d/%02d/%02d-%02d:%02d:%02d.%06d %llx ",
	                    t.tm_year + 1900,
	                    t.tm_mon + 1,
	                    t.tm_mday,
	                    t.tm_hour,
	                    t.tm_min,
	                    t.tm_sec,
	                    static_cast<int>(now_tv.tv_usec),
	                    static_cast<long long unsigned int>(thread_id));

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
	      fwrite(base, 1, p - base, file_);
	      fflush(file_);
	      if (base != buffer) {
	      	delete[] base;
	      }
	      break;
			}
		}
	};
}

#endif