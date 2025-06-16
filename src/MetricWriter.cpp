#include "metrics/MetricWriter.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>

#include "metrics/MetricCollection.h"

namespace metrics {

MetricWriter::MetricWriter(const std::string& filename) : _filename(filename) {}

static std::string makeTimestamp() {
  using clock = std::chrono::system_clock;
  auto now = clock::now();
  auto tt = clock::to_time_t(now);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) %
            1000;

  std::tm tm;
#ifdef _WIN32
  localtime_s(&tm, &tt);
#else
  localtime_r(&tt, &tm);
#endif
  char buf[32];
  std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
  std::ostringstream oss;
  oss << buf << '.' << std::setfill('0') << std::setw(3) << ms.count();
  return oss.str();
}

void MetricWriter::writeAll() {
  static std::mutex file_mutex;
  std::lock_guard<std::mutex> lock(file_mutex);
  std::ofstream out(_filename, std::ios::app);
  if (!out.is_open()) return;
  auto metrics = MetricCollection::instance().getAllMetrics();
  std::ostringstream line;
  line << makeTimestamp();

  for (const auto& metric : metrics) {
    std::string serialized = metric->serialize();
    if (!serialized.empty()) {
      line << ' ' << serialized;
    }
  }
  out << line.str() << '\n';
  for (const auto& metric : metrics) {
    metric->reset();
  }
}
}  // namespace metrics