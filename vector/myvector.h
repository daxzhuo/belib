
#ifndef _MY_VECTOR_H_
#define _MY_VECTOR_H_

#ifdef DEBUG
#include <iostream>
#define D(x) do {std::cerr << x;} while(0)
#else
#define D(x)
#endif // !DEBUG

#include <utility>
#include <malloc.h>

#define UNROLL_LOOP(first, last, OP) do { \
		for (; (last) - (first) >= 4; ++(first)) {	\
			OP((first));							\
			OP((first)+1);							\
			OP((first)+2);							\
			OP((first)+3);							\
		}											\
		for (;(first) != (last); ++(first)) OP((first));\
} while (0);

namespace dz {
	template <typename T>
	class myvector
	{
	public:
		myvector(size_t cap) :impl_(cap) {}


		struct Impl {
			typedef size_t size_type;
			typedef T*		pointer;

			pointer b_, e_, z_;

			Impl() : b_(nullptr), e_(nullptr), z_(nullptr) {}
			Impl(size_type n) { init(n); }
			Impl(Impl&& rhs): b_(rhs.b_), e_(rhs.e_), z_(rhs.z_) { 
				rhs.b_ = rhs.e_ = rhs.z_ = nullptr;
			}
			~Impl() { destory(); D("impl destory"); }

			// helper
			void swapData(Impl& rhs) {
				std::swap(b_, rhs.b_);
				std::swap(e_, rhs.e_);
				std::swap(z_, rhs.z_);
			}
			
			void init(size_type n) {
				if (n == 0)
					b_ = e_ = e_ = nullptr;
				else {
					b_ = static_cast<T*>(malloc(n*sizeof(T)));
					e_ = b_;
					z_ = b_ + n;
				}
			}
			void destory_range(T* first, T* last) {
				// Todo if (!boost::has_trivial_destructor<T>)
#define D_OP(p) (p)->~T();
				UNROLL_LOOP(first, last, D_OP)
#undef  D_OP
			}
			void destory() {
				if (b_) {
					destory_range(b_, e_);
					free(b_);
				}
			}

			void set(pointer new_b, size_type new_size, size_type new_cap) {
				e_ = new_b + new_size;
				z_ = new_b + new_cap;
				b_ = new_b;
			}

			void reset(size_type new_cap) {
				destory();
				init(new_cap);
			}

			void reset() {
				destory();
				b_ = e_ = z_ = nullptr;
			}
		};

		static void swap(Impl& lhs, Impl& rhs) {
			lhs.swapData(rhs);
		}

	public:

		typedef T*			iterator;
		typedef const T*	const_iterator;
		typedef size_t		size_type;
		typedef T			value_type;
		typedef value_type& reference;

		size_type size() const noexcept {
			return impl_.e_ - impl_.b_;
		}

		size_type capacity() const noexcept {
			return impl_.z_ - impl_.b_;
		}

		bool is_empty() const noexcept {
			return impl_.b_ == impl_.e_;
		}

		iterator begin() {
			return impl_.b_;
		}

		size_type compute_new_cap() {
			return capacity() * 2;
		}

		template <typename... Args>
		void emplace_back_aux(Args&&... args);
		

		void push_back(const T& value) {
			if (impl_.e_ != impl_.z_) {
				*impl_.e_ = value;
				++impl_.e_;
			}
			else {
				emplace_back_aux(value);
			}
		}

		iterator insert(iterator i, const T& value) {
			if (impl_.e_ == impl_.z_) {
				size_type sz = compute_new_cap();
				size_type old_sz = size();
				void *p;
				if ((p = realloc(impl_.b_, sz * sizeof(T))) != nullptr) {
					impl_.b_ = static_cast<T*>(p);
					impl_.e_ = impl_.b_ + old_sz;
					impl_.z_ = impl_.b_ + sz;
				}
			}
			iterator j = impl_.e_;
			while (j != i) {
				*j = *(j-1);
				--j;
			}
			*j = value;
			++impl_.e_;
			return j;
		}


	private:
		Impl impl_;
	};

	template <typename T>
	template <typename... Args>
	void myvector<T>::emplace_back_aux(Args&&... args) {
		size_type sz = compute_new_cap();
		size_type old_sz = size();
		void *p;
		if ((p = realloc(impl_.b_, sz * sizeof(T))) != nullptr) {
			impl_.b_ = static_cast<T*>(p);
			impl_.e_ = impl_.b_ + old_sz;
			impl_.z_ = impl_.b_ + sz;
			new (impl_.e_) T(std::forward<Args>(args)...);
			++impl_.e_;
		}
		else {
			D("emplace_back_aux realloc fail...");
		}
	}
}
#endif // !_MY_VECTOR_H_




