

#ifndef _NONCOPYABLE_H_
#define _NONCOPYABLE_H_

// taken from boost
class noncopyable {
protected:
  constexpr noncopyable() = default;
  ~noncopyable() = default;
  noncopyable(const noncopyable &) = delete;
  noncopyable &operator=(const noncopyable &) = delete;
};

#endif // BOOST_NONCOPYABLE_HPP_INCLUDED