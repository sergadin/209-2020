#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

class LogDuration {
public:
  explicit LogDuration(const std::string &msg = "")
      : message(msg + ": "), start(std::chrono::steady_clock::now()) {}

  ~LogDuration() {
    using namespace std::chrono;
    auto finish = std::chrono::steady_clock::now();
    auto dur = finish - start;
    std::ostringstream os;
    os << message;
    if (duration_cast<nanoseconds>(dur).count() < 1000)
      os << duration_cast<nanoseconds>(dur).count() << " ns";
    else if (duration_cast<microseconds>(dur).count() < 1000) {
      if (duration_cast<microseconds>(dur).count() < 10)
        os << std::setprecision(4)
           << duration_cast<nanoseconds>(dur).count() * 1.0 / 1000.0 << " us";
      else
        os << duration_cast<microseconds>(dur).count() << " us";
    } else if (duration_cast<milliseconds>(dur).count() < 1000) {
      if (duration_cast<milliseconds>(dur).count() < 10)
        os << std::setprecision(4)
           << duration_cast<microseconds>(dur).count() * 1.0 / 1000.0 << " ms";
      else
        os << duration_cast<milliseconds>(dur).count() << " ms";
    } else {
      if (duration_cast<seconds>(dur).count() < 10)
        os << std::setprecision(4)
           << duration_cast<milliseconds>(dur).count() * 1.0 / 1000.0 << " s";
      else
        os << duration_cast<seconds>(dur).count() << " s";
    }
    os << std::endl;
    std::cerr << os.str();
  }

private:
  std::string message;
  std::chrono::steady_clock::time_point start;
};

#ifndef UNIQ_ID
#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)
#endif

#define LOG_DURATION(message) LogDuration UNIQ_ID(__LINE__){message};
