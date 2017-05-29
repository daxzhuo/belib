#include <sstream>

class StringStream {
  StringStream& operator<<(const std::string& value);
  StringStream& operator<<(const char* value);
  StringStream& operator<<(char value);
  StringStream& operator<<(int value);
  StringStream& operator<<(unsigned int value);
  StringStream& operator<<(long value);
  StringStream& operator<<(unsigned long value);
  StringStream& operator<<(long long value);
  StringStream& operator<<(unsigned long long value);
  StringStream& operator<<(double value);
  StringStream& operator<<(void* value);
 private:
  Buffer buffer_;
  
};