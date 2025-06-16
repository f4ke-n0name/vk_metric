#include "metrics/MetricCollection.h"
#include <shared_mutex>
#include <mutex>

namespace metrics {

MetricCollection& MetricCollection::instance() {
  static MetricCollection instance;
  return instance;
}

MetricCollection::MetricCollection() = default;

void MetricCollection::registerMetric(std::shared_ptr<IMetric> metric) {
  std::unique_lock lock(_mutex);
  _metrics.push_back(std::move(metric));
}

std::vector<std::shared_ptr<IMetric>> MetricCollection::getAllMetrics() {
  std::shared_lock lock(_mutex);
  return _metrics;
}

}  // namespace metrics