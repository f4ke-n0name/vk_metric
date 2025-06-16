# vk_metric

## Быстрый старт

### 1. Добавьте метрику

```cpp
#include "metrics/Metric.h"
#include "metrics/MetricCollection.h"

using namespace metrics;

auto cpu = std::make_shared<Metric<double>>(
    "CPU", [](double v) { return v >= 0.0 && v <= 2.0; });
MetricCollection::instance().registerMetric(cpu);
cpu->update(1.53);
```

### 2. Запишите метрики в файл

```cpp
#include "metrics/MetricWriter.h"

MetricWriter writer("metrics.txt");
writer.writeAll(); // Все метрики сериализуются и сбрасываются
```

### 3. Пример main.cpp

См. [examples/main.cpp](examples/main.cpp)

## Формат строки метрики

```
2025-06-16 12:34:56.789 "CPU" 1.0 "HTTP requests RPS" 42
```

## Как реализовать свою метрику или валидатор

Вы можете реализовать собственную метрику, унаследовавшись от интерфейса `IMetric`:

```cpp
class CustomMetric : public metrics::IMetric {
 public:
  std::string serialize() const override {
    // логика сериализации
  }
  void reset() override {
    // логика сброса
  }
  std::string name() const override {
    return "custom_metric";
  }
};
```

Также можно задать свой валидатор для метрики:

```cpp
auto my_metric = std::make_shared<metrics::Metric<int>>(
    "positive_metric", [](int v) { return v > 0; });
```

## Как собрать

```sh
git clone <repo>
cd vk
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

### Важно: настройка путей компиляторов

Если CMake не может найти компиляторы или вы используете MinGW/MSYS2, возможно, потребуется явно указать пути к компиляторам.
Пример:

```sh
cmake .. -G "MinGW Makefiles" -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe
```

Убедитесь, что выбранные компиляторы доступны в вашей системе и соответствуют вашей платформе.

## Зависимости

- C++17 или новее
- CMake

## Структура

```
include/metrics/    # Заголовочные файлы
src/                # Реализация
examples/           # Пример использования
tests/              # Unit-тесты
```

## Тестирование

```sh
ctest
```
