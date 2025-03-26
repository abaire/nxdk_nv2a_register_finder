#ifndef SETSPECULARENABLE_H
#define SETSPECULARENABLE_H

#include "test_case.h"

struct SetSpecularEnable : public TestCase {

  explicit SetSpecularEnable(FILE *log_stream = nullptr)
      : TestCase("NV097_SET_SPECULAR_ENABLE", log_stream) {}

  void Run() override;
};

#endif // SETSPECULARENABLE_H
