


class Buffer {
 public:
  void prepend(const void* data, size_t len) {
    assert(len <= prependableBytes());
    readerIndex_ -= len;
    const char* d = static_cast<const char*>(data);
    std::copy(d, d+len, begin()+readerIndex_);
  }
 private:
  char* begin() {
    return buffer_.data();
  }
  const char* begin() const {
    return buffer_.data();
  }
  std::vector<char> buffer_;
  size_t readerIndex;
  size_t writerIndex;
};