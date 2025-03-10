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
