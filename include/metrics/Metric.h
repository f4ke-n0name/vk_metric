#pragma once
#include <atomic>
#include <functional>
#include <mutex>
#include <sstream>
#include <string>
#include <stdexcept>

#include "IMetric.h"

namespace metrics {

template <typename T>
class Metric : public IMetric {
 public:
  using Validator = std::function<bool(const T&)>;

  Metric(
      std::string name, Validator validator = [](const T&) { return true; })
      : name_(std::move(name)),
        validator_(std::move(validator)),
        value_{},
        hasValue_(false) {}

  void update(const T& value) {
    if (!validator_(value)) {
      throw std::invalid_argument("Validation failed for metric: " + name_);
    }
    std::lock_guard<std::mutex> lock(mutex_);
    value_ = value;
    hasValue_ = true;
  }

  std::string serialize() const override {
    std::lock_guard<std::mutex> lock(mutex_);
    if (hasValue_) {
      std::ostringstream oss;
      oss << "\"" << name_ << "\" " << value_;
      return oss.str();
    }
    return "";
  }

  std::string name() const override { return name_; }

  void reset() override {
    std::lock_guard<std::mutex> lock(mutex_);
    hasValue_ = false;
  }

 private:
  std::string name_;
  Validator validator_;
  T value_;
  bool hasValue_;
  mutable std::mutex mutex_;
};

}  // namespace metrics
