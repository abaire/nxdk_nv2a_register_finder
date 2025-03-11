#include "test_case.h"

#include <pbkit/pbkit.h>

void TestCase::Initialize() {
  if (log_stream_) {
    fprintf(log_stream_, "BEGIN %s\n", name_.c_str());
  }
}

void TestCase::Cleanup() {
  if (log_stream_) {
    fprintf(log_stream_, "END %s\n\n", name_.c_str());
  }
}

void TestCase::PBKitBusyWait() {
  while (pb_busy()) {
    /* Wait for completion... */
  }
}

PGRAPHDiffToken TestCase::StartTracking() const { return {true, log_stream_}; }

void TestCase::StopTracking(const PGRAPHDiffToken &token) const {
  token.DumpDiff();
}

void TestCase::ApplyAndLog(const std::string &name,
                           std::function<void()> apply_func,
                           size_t iterations) const {

  fprintf(log_stream_, "\n> %s\n", name.c_str());

  for (auto i = 0; i < iterations; ++i) {

    fprintf(log_stream_, "! ITERATION %d\n", i);

    auto token = StartTracking();
    apply_func();
    PBKitBusyWait();
    StopTracking(token);
    fprintf(log_stream_, "\n");
  }

  fprintf(log_stream_, "\n");
}
