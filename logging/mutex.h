
#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>


class Mutex
{
public:
	Mutex() {
		pthread_mutex_init(&mu_, NULL);
	}
	~Mutex() {
		pthread_mutex_destroy(&mu_);
	}
	void lock() {
		pthread_mutex_lock(&mu_);
	}

	pthread_mutex_t* get() {
		return &mu_;
	}

	void unlock() {
		pthread_mutex_unlock(&mu_);
	}

private:
	pthread_mutex_t mu_;
	
};

class LockGuard {
public:
	explicit LockGuard(Mutex& mu) : mu_(mu) {
		mu_.lock();
	}

	~LockGuard() { mu_.unlock(); }

private:
	Mutex& mu_;
};

#define LockGuard(x) error "Bad use of LockGuard"

#endif



