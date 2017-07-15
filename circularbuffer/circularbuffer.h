


#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <utility>
#include <iostream>

template <typename T, size_t maxsz_bits>
class CircularBuffer
{
public:
	static constexpr size_t bufsz = 1 << maxsz_bits;
	static constexpr size_t maxsz = bufsz - 1;
	// -1 to make sure that head==tail alway means empty
	static constexpr size_t mask = maxsz;

	//CircularBuffer() : head(0), tail(0) {}


	size_t size() {
		return head - tail + ((size_t)((head >= tail) - 1) & bufsz);
	}

	bool is_full() {
		return size() == bufsz;
	}

	void push(T&& t) {
    assert(size() < maxsz);
    new(tbuffer(head)) T(std::move(t));
    head = ( head + 1 ) & mask; 
  } 
 
  T pop() {
    assert(size() > 0);
    T* ttail = tbuffer(tail);
    T ret = std::move(*ttail);
    ttail->~T();
    tail = ( tail + 1 ) & mask;
    return ret;
  }
 
private:
  T* tbuffer(size_t idx) {
    return reinterpret_cast<T*>(buffer + (idx*sizeof(T)));
  }

	size_t head=0;
	size_t tail=0;
	alignas(T) uint8_t buffer[bufsz];
};




#endif //_CIRCULAR_BUFFER_H_