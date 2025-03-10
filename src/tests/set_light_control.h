#ifndef SETLIGHTCONTROL_H
#define SETLIGHTCONTROL_H

#include "test_case.h"

struct SetLightControl : public TestCase {

  explicit SetLightControl(FILE *log_stream = nullptr)
      : TestCase("NV097_SET_LIGHT_CONTROL", log_stream) {}

  void Run() override;
};

#endif // SETLIGHTCONTROL_H
