#include "set_specular_params.h"

#include <pbkit/pbkit.h>

#   define NV097_SET_SPECULAR_PARAMS                          0x000009E0
#   define NV097_SET_SPECULAR_PARAMS_BACK                     0x00001E28

void SetSpecularParams::Run() {

  auto reset = []() {
    auto p = pb_begin();
    p = pb_push1(p, NV097_SET_SPECULAR_PARAMS, 0);
    pb_end(p);
    PBKitBusyWait();
  };

  reset();

  char buf[64];
  snprintf(buf, sizeof(buf), "NV097_SET_SPECULAR_PARAMS 0x0EBAEBA0");
  ApplyAndLog(buf, []() {
    auto p = pb_begin();
    p = pb_push1(p, NV097_SET_SPECULAR_PARAMS, 0x0EBAEBA0);
    pb_end(p);
  });

  snprintf(buf, sizeof(buf), "NV097_SET_SPECULAR_PARAMS 0x0");
  ApplyAndLog(buf, []() {
    auto p = pb_begin();
    p = pb_push1(p, NV097_SET_SPECULAR_PARAMS, 0);
    pb_end(p);
  });

  snprintf(buf, sizeof(buf), "NV097_SET_SPECULAR_PARAMS 0xDEADBEEF");
  ApplyAndLog(buf, []() {
    auto p = pb_begin();
    p = pb_push1(p, NV097_SET_SPECULAR_PARAMS, 0xDEADBEEF);
    pb_end(p);
  });


  reset();
}
