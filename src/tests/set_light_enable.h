#ifndef SETLIGHTENABLE_H
#define SETLIGHTENABLE_H

#include "test_case.h"

struct SetLightEnable : public TestCase {

  explicit SetLightEnable(FILE *log_stream = nullptr)
      : TestCase("NV097_SET_LIGHTING_ENABLE", log_stream) {}

  void Run() override;
};

#endif // SETLIGHTENABLE_H
