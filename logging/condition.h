
#ifndef CONDITION_H_
#define CONDITION_H_

#include "Mutex.h"
#include <pthread.h>
#include <time.h>

class Condition {
public:
  explicit Condition(Mutex &mu) : mu_(mu) { pthread_cond_init(&cond_, NULL); }
  ~Condition() { pthread_cond_destroy(&cond_); }

  void wait() { pthread_cond_wait(&cond_, mu_.get()); }

  void notify() { pthread_cond_signal(&cond_); }

  void notifyAll() { pthread_cond_broadcast(&cond_); }

  bool waitforSeconds(int seconds);

private:
  Mutex &mu_;
  pthread_cond_t cond_;
};

bool Condition::waitforSeconds(int seconds) {
  struct timespec abstime;
  clock_gettime(CLOCK_REALTIME, &abstime);

  abstime.tv_sec += static_cast<time_t>(abstime.tv_nsec + seconds);
  return ETIMEDOUT == pthread_cond_timedwait(&cond_, mu_.get(), &abstime);
}

#endif
