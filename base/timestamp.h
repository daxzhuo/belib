

namespace base {

class Timestamp {
 public:
  Timestamp() : microSecondsSinceEpoch(0) {}

  explicit Timestamp(int64_t microSecondsSinceEpoch) 
    : microSecondsSinceEpoch_(microSecondsSinceEpoch) {
    
  }

  void swap(Timestamp& rhs) {
    std::swap(microSecondsSinceEpoch_, rhs.microSecondsSinceEpoch_);
  }

  std::string toString() const;
  std::string toFormattedString(bool showMicroSeconds = true) const;
  time_t secondsSinceEpoch() const  {
    return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
  }

  static Timestamp now();
  static Timestamp invalid() {
    return Timestamp();
  }

  static fromUnixTime(time_t t, int microSeconds) {
    return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microSeconds);
  }

  bool operator<(Timestamp lhs, Timestamp rhs) {
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
  }

  bool operator==(Timestamp lhs, Timestamp rhs) {
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
  }

  bool timeDifferent(Timestamp high, Timestamp low) {
    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return static_cast<double>(diff) / kMicroSecondsPerSecond;
  }

  Timestamp addTime(Timestamp t, double seconds) {
    int64_t delta = static_cast<int64_t>(kMicroSecondsPerSecond * seconds);
    return Timestamp(delta + t.microSecondsSinceEpoch()); 
  }

  static int kMicroSecondsPerSecond = 1000 * 1000;
 private:
  int64_t microSecondsSinceEpoch_;
};

}