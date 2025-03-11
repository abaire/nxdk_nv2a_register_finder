#include "set_light_control.h"

#include <pbkit/pbkit.h>

void SetLightControl::Run() {

  auto reset = []() {
    auto p = pb_begin();
    p = pb_push1(p, NV097_SET_LIGHT_CONTROL, 0);
    pb_end(p);
    PBKitBusyWait();
  };

  reset();
  // #       define NV097_SET_LIGHT_CONTROL_V_INCLUDE_SPECULAR         0
  // #       define NV097_SET_LIGHT_CONTROL_V_SEPARATE_SPECULAR        1
  // #       define NV097_SET_LIGHT_CONTROL_V_LOCALEYE                 (1 << 16)
  // #       define NV097_SET_LIGHT_CONTROL_V_ALPHA_OPAQUE             0
  // #       define  (1 << 17)

  char buf[64];
  snprintf(buf, sizeof(buf), "NV097_SET_LIGHT_CONTROL_V_SEPARATE_SPECULAR 0x%X",
           NV097_SET_LIGHT_CONTROL_V_SEPARATE_SPECULAR);
  ApplyAndLog(buf, []() {
    auto p = pb_begin();
    p = pb_push1(p, NV097_SET_LIGHT_CONTROL,
                 NV097_SET_LIGHT_CONTROL_V_SEPARATE_SPECULAR);
    pb_end(p);
  });
  reset();

  snprintf(buf, sizeof(buf), "NV097_SET_LIGHT_CONTROL_V_LOCALEYE 0x%X",
           NV097_SET_LIGHT_CONTROL_V_LOCALEYE);
  ApplyAndLog(buf, []() {
    auto p = pb_begin();
    p = pb_push1(p, NV097_SET_LIGHT_CONTROL,
                 NV097_SET_LIGHT_CONTROL_V_LOCALEYE);
    pb_end(p);
  });
  reset();

  snprintf(buf, sizeof(buf),
           "NV097_SET_LIGHT_CONTROL_V_ALPHA_FROM_MATERIAL_SPECULAR 0x%X",
           NV097_SET_LIGHT_CONTROL_V_ALPHA_FROM_MATERIAL_SPECULAR);
  ApplyAndLog(buf, []() {
    auto p = pb_begin();
    p = pb_push1(p, NV097_SET_LIGHT_CONTROL,
                 NV097_SET_LIGHT_CONTROL_V_ALPHA_FROM_MATERIAL_SPECULAR);
    pb_end(p);
  });
  reset();
}
