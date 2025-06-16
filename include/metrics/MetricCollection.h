#pragma once
#include <memory>
#include <shared_mutex>
#include <vector>

#include "IMetric.h"

namespace metrics {

class MetricCollection {
 public:
  static MetricCollection& instance();

  void registerMetric(std::shared_ptr<IMetric> metric);
  std::vector<std::shared_ptr<IMetric>> getAllMetrics();

 private:
  MetricCollection();
  std::vector<std::shared_ptr<IMetric>> _metrics;
  mutable std::shared_mutex _mutex;
};

}  // namespace metrics
