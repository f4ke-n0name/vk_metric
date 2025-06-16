#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <vector>
#include <string>

#include "metrics/MetricCollection.h"
#include "metrics/IMetric.h"

using namespace metrics;
using ::testing::Return;

class MockMetric : public IMetric {
 public:
  MOCK_METHOD(std::string, serialize, (), (const, override));
  MOCK_METHOD(void, reset, (), (override));
  MOCK_METHOD(std::string, name, (), (const, override));
};


#include <gtest/gtest.h>
#include "metrics/Metric.h"

using namespace metrics;

TEST(MetricTest, ThrowsOnInvalidValue) {
    Metric<int> m("test_metric", [](const int& v) { return v > 0; });
    EXPECT_THROW(m.update(-5), std::invalid_argument);
}

TEST(MetricTest, StoresValidValue) {
    Metric<int> m("test_metric");
    m.update(1);
    EXPECT_NE(m.serialize().find("1"), std::string::npos);
    m.update(2);
    EXPECT_NE(m.serialize().find("2"), std::string::npos);
    EXPECT_EQ(m.serialize().find("1"), std::string::npos);
}