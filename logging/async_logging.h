//
// Created by dazhuo on 11/5/16.
//

#ifndef LOGGING_ASYNCLOGGING_H
#define LOGGING_ASYNCLOGGING_H

#include "buffer.h"
#include "Condition.h"
#include "Mutex.h"
#include <memory>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <utility>
#include <vector>
namespace logging {
class AsyncLogging {
public:
  AsyncLogging(FILE *file, int flushInterval)
      : flushInterval_(flushInterval), running_(false), cond_(mu_),
        curbuf_(new Buffer), nextbuf_(new Buffer), file_(file) {
    curbuf_->bzero();
    nextbuf_->bzero();
  }

  void start() {
    running_ = true;
    pthread_create(&thread_, NULL, &ThreadFunc, this);
  }
  void stop() {
    running_ = false;
    cond_.notify();
    pthread_join(thread_, NULL);
  }
  void append(const char *data, int len);
  static void *ThreadFunc(void *arg) {
    ((AsyncLogging *)arg)->BgThread();
    return NULL;
  }
  void BgThread(void);

private:
  typedef detail::FixedBuffer<detail::lbuffersize> Buffer;
  typedef std::unique_ptr<Buffer> BufferPtr;
  bool running_;
  const int flushInterval_;

  Mutex mu_;
  Condition cond_;
  pthread_t thread_;
  BufferPtr nextbuf_;
  BufferPtr curbuf_;
  std::vector<BufferPtr> buffers_;
  FILE *file_;
};

void AsyncLogging::append(const char *data, int len) {
  LockGuard l(mu_);
  bool ok = curbuf_->append(data, len);

  if (!ok) {
    // not enough space
    buffers_.push_back(std::move(curbuf_));
    if (nextbuf_) {
      curbuf_ = std::move(nextbuf_);
    } else {
      curbuf_.reset(new Buffer);
    }

    ok = curbuf_->append(data, len);
    assert(ok);
    cond_.notify();
  }
}

void AsyncLogging::BgThread() {
  assert(running_ == true);
  BufferPtr newbuf1(new Buffer);
  BufferPtr newbuf2(new Buffer);
  newbuf1->bzero();
  newbuf2->bzero();
  std::vector<BufferPtr> bufToWrite;
  while (running_) {
    assert(newbuf2 && newbuf2->length() == 0);
    assert(newbuf1 && newbuf1->length() == 0);
    assert(bufToWrite.empty());
    {

      LockGuard l(mu_);
      if (buffers_.empty()) {
        cond_.waitforSeconds(flushInterval_);
      }
      buffers_.push_back(std::move(curbuf_));
      curbuf_ = std::move(newbuf1);
      bufToWrite.swap(buffers_);
      if (!nextbuf_) {
        nextbuf_ = std::move(newbuf2);
      }
    }
    assert(!bufToWrite.empty());

    for (size_t i = 0; i < bufToWrite.size(); ++i) {
      fwrite(bufToWrite[i]->data(), 1, bufToWrite[i]->length(), file_);
    }

    if (bufToWrite.size() > 2) {
      // drop non zeroed buffer, avoid trashing
      bufToWrite.resize(2);
    }

    if (!newbuf1) {
      assert(!bufToWrite.empty());
      newbuf1 = std::move(bufToWrite.back());
      bufToWrite.pop_back();
      newbuf1->reset();
    }
    if (!newbuf2) {
      assert(!bufToWrite.empty());
      newbuf2 = std::move(bufToWrite.back());
      bufToWrite.pop_back();
      newbuf2->reset();
    }
    bufToWrite.clear();
    fflush(file_);
  }
  fflush(file_);
  // may be some buffer still in buffers
}
}

#endif // LOGGING_ASYNCLOGGING_H
