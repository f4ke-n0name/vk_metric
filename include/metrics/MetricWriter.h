#pragma once
#include <string>

namespace metrics {

class MetricWriter {
 public:
  explicit MetricWriter(const std::string& filename);
  void writeAll();

 private:
  std::string _filename;
};

}  // namespace metrics