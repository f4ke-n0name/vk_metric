cmake_minimum_required(VERSION 3.10)
project(vk)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(metrics
    src/MetricCollection.cpp
    src/MetricWriter.cpp
)
target_include_directories(metrics PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(example examples/main.cpp)
target_link_libraries(example PRIVATE metrics)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(MetricWriterTest tests/MetricWriterTest.cpp)
set_target_properties(MetricWriterTest PROPERTIES WIN32_EXECUTABLE OFF)
target_link_libraries(MetricWriterTest PRIVATE metrics gtest_main gmock)
add_test(NAME MetricWriterTest COMMAND MetricWriterTest)

add_executable(MetricCollectionTest tests/MetricCollectionTest.cpp)
set_target_properties(MetricCollectionTest PROPERTIES WIN32_EXECUTABLE OFF)
target_link_libraries(MetricCollectionTest PRIVATE metrics gtest_main gmock)
add_test(NAME MetricCollectionTest COMMAND MetricCollectionTest)