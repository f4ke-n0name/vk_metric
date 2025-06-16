#include <memory>
#include <climits>

#include "metrics/Metric.h"
#include "metrics/MetricCollection.h"
#include "metrics/MetricWriter.h"

using namespace metrics;

int main() {
  auto cpu = std::make_shared<Metric<double>>(
      "CPU", [](double v) { return v >= 0.0 && v <= 2.0; });
  auto httprps = std::make_shared<Metric<int>>(
      "HTTP requests RPS", [](int v) { return v >= 0 && v <= INT_MAX; });
  auto ram = std::make_shared<Metric<std::string>>(
      "RAM MB");

  MetricCollection::instance().registerMetric(cpu);
  MetricCollection::instance().registerMetric(httprps);
  MetricCollection::instance().registerMetric(ram);

  cpu->update(0.97);
  httprps->update(42);
  MetricWriter writer("metrics.txt");
  writer.writeAll();

  cpu->update(1.12);
  httprps->update(30);
  writer.writeAll();

  cpu->update(1.78);
  httprps->update(102);
  ram->update("153706");
  writer.writeAll();

  return 0;
}
