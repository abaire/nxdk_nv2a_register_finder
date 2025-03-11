#ifndef TESTCASE_H
#define TESTCASE_H

#include <cstdio>
#include <functional>
#include <string>

#include "pgraph_diff_token.h"

struct TestCase {
  TestCase(const std::string &name, FILE *log_stream)
      : name_{name}, log_stream_{log_stream} {}
  virtual ~TestCase() = default;

  virtual void Initialize();
  virtual void Run() = 0;
  virtual void Cleanup();

  static void PBKitBusyWait();

protected:
  void ApplyAndLog(const std::string &name, std::function<void()> apply_func,
                   size_t iterations = 3) const;
  PGRAPHDiffToken StartTracking() const;
  void StopTracking(const PGRAPHDiffToken &token) const;

protected:
  std::string name_;
  FILE *log_stream_;
};

#endif // TESTCASE_H
