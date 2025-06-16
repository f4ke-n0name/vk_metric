#pragma once
#include <string>

namespace metrics {

class IMetric {
 public:
  virtual ~IMetric() = default;
  virtual std::string name() const = 0;
  virtual void reset() = 0;
  virtual std::string serialize() const = 0;
};

}