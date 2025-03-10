#ifndef TESTCASE_H
#define TESTCASE_H

#include <cstdio>
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
  PGRAPHDiffToken StartTracking() const;
  void StopTracking(const PGRAPHDiffToken &token) const;

protected:
  std::string name_;
  FILE *log_stream_;
};

#endif // TESTCASE_H
