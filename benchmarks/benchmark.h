///////////////////////////////////////////////////////////////////////////////
// @file
// @copyright (C) 2020
//    All rights reserved.
//
// Use, modification, and distribution is subject to the license.
//
// @author Sergiy Gogolenko <sergiy.gogolenko@gmail.com>
///////////////////////////////////////////////////////////////////////////////
// Basic timing macro
///////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef HIGIS_BENCHMARKS_BENCHMARK_H_
#define HIGIS_BENCHMARKS_BENCHMARK_H_

#include <chrono>
#include <ostream>
#include <time.h>

namespace benchmark {
namespace chrono {
class Timing final {
public:
  Timing() : beg{std::chrono::high_resolution_clock::now()}, end{} {}
  inline void start() noexcept {
    beg = std::chrono::high_resolution_clock::now();
  }
  inline void stop() noexcept {
    end = std::chrono::high_resolution_clock::now();
  }
  double duration() const noexcept {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg)
               .count() *
           1.e-6;
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> beg;
  std::chrono::time_point<std::chrono::high_resolution_clock> end;
};

} // namespace chrono

namespace cclock {
class Timing final {
public:
  Timing() : end{} { clock_gettime(CLOCK_MONOTONIC, &beg); }
  inline void start() noexcept { clock_gettime(CLOCK_MONOTONIC, &beg); }
  inline void stop() noexcept { clock_gettime(CLOCK_MONOTONIC, &end); }
  double duration() const noexcept {
    return 1.e3 * (end.tv_sec - beg.tv_sec) +
           1.e-6 * (end.tv_nsec - beg.tv_nsec);
  }

private:
  struct timespec beg;
  struct timespec end;
};

} // namespace cclock
} // namespace benchmark

namespace bench = benchmark::cclock;
#define BENCH_CALL_TIMING(_)                                                   \
  {                                                                            \
    bench::Timing _timer;                                                      \
    _;                                                                         \
    _timer.stop();                                                             \
    _timer.duration();                                                         \
    std::cerr << "BENCH:" << _timer.duration() << "ms:\"" << #_ << "\""        \
              << std::endl;                                                    \
  }

#endif // HIGIS_BENCHMARKS_BENCHMARK_H_
