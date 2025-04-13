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


// Used to set light parameters for lights beyond 0.
// Supported params:
//    NV097_SET_LIGHT_AMBIENT_COLOR
//    NV097_SET_LIGHT_DIFFUSE_COLOR
//    NV097_SET_LIGHT_SPECULAR_COLOR
//    NV097_SET_LIGHT_LOCAL_RANGE
//    NV097_SET_LIGHT_INFINITE_HALF_VECTOR
//    NV097_SET_LIGHT_INFINITE_DIRECTION
//    NV097_SET_LIGHT_SPOT_FALLOFF
//    NV097_SET_LIGHT_SPOT_DIRECTION
//    NV097_SET_LIGHT_LOCAL_POSITION
//    NV097_SET_LIGHT_LOCAL_ATTENUATION
#define SET_LIGHT(i, param) ((param) + (i) * 128)

// Used to set light parameters for back lights beyond 0.
// Supported params:
//    NV097_SET_BACK_LIGHT_AMBIENT_COLOR
//    NV097_SET_BACK_LIGHT_DIFFUSE_COLOR
//    NV097_SET_BACK_LIGHT_SPECULAR_COLOR
#define SET_BACK_LIGHT(i, param) ((param) + (i) * 64)

#endif // TESTCASE_H
