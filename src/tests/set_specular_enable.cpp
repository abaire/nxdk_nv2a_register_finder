#include "set_specular_enable.h"

#include <pbkit/pbkit.h>

void SetSpecularEnable::Run() {

  auto reset = []() {
    auto p = pb_begin();
    p = pb_push1(p, NV097_SET_SPECULAR_ENABLE, 0);
    pb_end(p);
    PBKitBusyWait();
  };

  reset();

  char buf[64];
  snprintf(buf, sizeof(buf), "NV097_SET_SPECULAR_ENABLE 0x1");
  ApplyAndLog(buf, []() {
    auto p = pb_begin();
    p = pb_push1(p, NV097_SET_SPECULAR_ENABLE, 1);
    pb_end(p);
  });

  // Allow the 1 value to bleed over into the next test so that there's a
  // modification.

  snprintf(buf, sizeof(buf), "NV097_SET_SPECULAR_ENABLE 0x0");
  ApplyAndLog(buf, []() {
    auto p = pb_begin();
    p = pb_push1(p, NV097_SET_SPECULAR_ENABLE, 0);
    pb_end(p);
  });
  reset();
}
