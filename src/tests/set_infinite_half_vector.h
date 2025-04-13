#ifndef SETINFINITEHALFVECTOR_H
#define SETINFINITEHALFVECTOR_H

#include "test_case.h"

struct SetInfiniteHalfVector : public TestCase {

  explicit SetInfiniteHalfVector(FILE *log_stream = nullptr)
      : TestCase("NV097_SET_LIGHT_INFINITE_HALF_VECTOR", log_stream) {}

  void Run() override;

  // Handled in Run
  void Initialize() override {}
  void Cleanup() override {}
};

#endif // SETINFINITEHALFVECTOR_H
