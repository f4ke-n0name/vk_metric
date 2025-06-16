#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <atomic>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

#include "metrics/IMetric.h"
#include "metrics/MetricCollection.h"
#include "metrics/MetricWriter.h"

using ::testing::_;
using ::testing::Return;

namespace metrics {

class ThreadSafeMetric : public IMetric {
 public:
  std::atomic<int> serialize_count{0};
  std::atomic<int> reset_count{0};

  std::string serialize() const override {
    ++const_cast<ThreadSafeMetric*>(this)->serialize_count;
    return "thread";
  }
  void reset() override { ++reset_count; }
  std::string name() const override { return "thread"; }
};

class SimulatedEventMetric : public IMetric {
 public:
  std::atomic<int> counter{0};

  std::string serialize() const override {
    return "events=" + std::to_string(counter.load());
  }
  void reset() override {}
  std::string name() const override { return "events"; }
};

TEST(MetricWriterThreadTest, ConcurrentWriteAll) {
  const std::string filename = "test_thread.log";
  std::remove(filename.c_str());
  auto metric = std::make_shared<ThreadSafeMetric>();
  MetricCollection::instance().registerMetric(metric);
  MetricWriter writer(filename);
  constexpr int kThreadCount = 4;
  std::vector<std::thread> threads;
  for (int i = 0; i < kThreadCount; ++i) {
    threads.emplace_back([&writer]() { writer.writeAll(); });
  }
  for (auto& t : threads) t.join();

  std::ifstream in(filename);
  std::string line;
  int count = 0;
  while (std::getline(in, line)) {
    EXPECT_THAT(line, ::testing::HasSubstr("thread"));
    ++count;
  }

  EXPECT_EQ(count, kThreadCount);
  EXPECT_EQ(metric->serialize_count, kThreadCount);
  EXPECT_EQ(metric->reset_count, kThreadCount);
}

TEST(MetricWriterThreadTest, WriteAllDoesNotBlockEventThreads) {
  const std::string filename = "test_async.log";
  std::remove(filename.c_str());
  auto metric = std::make_shared<SimulatedEventMetric>();
  MetricCollection::instance().registerMetric(metric);
  MetricWriter writer(filename);
  std::atomic<bool> stop{false};
  std::thread writerThread([&]() {
    while (!stop) {
      writer.writeAll();
    }
  });

  const int kEventIterations = 100000;
  for (int i = 0; i < kEventIterations; ++i) {
    ++metric->counter;
  }

  stop = true;
  writerThread.join();
  ASSERT_GE(metric->counter, kEventIterations);
}

}  // namespace metrics