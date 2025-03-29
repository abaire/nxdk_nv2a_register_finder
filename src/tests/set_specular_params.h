#ifndef SETSPECULARPARAMS_H
#define SETSPECULARPARAMS_H

#include "test_case.h"

struct SetSpecularParams : public TestCase {

  explicit SetSpecularParams(FILE *log_stream = nullptr)
      : TestCase("NV097_SET_SPECULAR_PARAMS", log_stream) {}

  void Run() override;
};

#endif // SETSPECULARPARAMS_H
