#ifndef NXDK_PGRAPH_TESTS_PGRAPH_DIFF_TOKEN_H
#define NXDK_PGRAPH_TESTS_PGRAPH_DIFF_TOKEN_H

#include <fstream>

#include "pbkit_ext.h"

struct PGRAPHDiffToken {
  uint8_t registers[PGRAPH_REGISTER_ARRAY_SIZE];

  PGRAPHDiffToken(bool initialize = true, FILE *log_stream = nullptr);

  void Capture();
  void DumpDiff() const;

private:
  FILE *log_stream_;
};

#endif  // NXDK_PGRAPH_TESTS_PGRAPH_DIFF_TOKEN_H
