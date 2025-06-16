#include <memory>

#include "metrics/Metric.h"
#include "metrics/MetricCollection.h"
#include "metrics/MetricWriter.h"

using namespace metrics;

int main() {
  auto cpu = std::make_shared<Metric<double>>(
      "CPU", [](double v) { return v >= 0.0 && v <= 2.0; });
  auto httprps = std::make_shared<Metric<int>>(
      "HTTP requests RPS", [](int v) { return v >= 0 && v <= INT_MAX; });

  MetricCollection::instance().registerMetric(cpu);
  MetricCollection::instance().registerMetric(httprps);
  cpu->update(0.97);
  httprps->update(42);
  MetricWriter writer("metrics.txt");
  writer.writeAll();
  cpu->update(1.12);
  httprps->update(30);
  writer.writeAll();
  return 0;
}
