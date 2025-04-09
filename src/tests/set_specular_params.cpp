#include "set_specular_params.h"

#include <pbkit/pbkit.h>

#   define NV097_SET_SPECULAR_PARAMS                          0x000009E0
#   define NV097_SET_SPECULAR_PARAMS_BACK                     0x00001E28

void SetSpecularParams::Run() {

  auto reset = [](int offset) {
    auto p = pb_begin();
    p = pb_push1(p, NV097_SET_SPECULAR_PARAMS + offset, 0);
    pb_end(p);
    PBKitBusyWait();
  };


  char buf[64];

  for (uint32_t i = 0; i < 6; ++i) {
    fprintf(log_stream_, "BEGIN NV097_SET_SPECULAR_PARAMS_%d\n", i);

    auto offset = 4 * i;
    reset(offset);

    snprintf(buf, sizeof(buf), "NV097_SET_SPECULAR_PARAMS_%d 0x0EBAEBA0", i);
    ApplyAndLog(buf, [offset]() {
      auto p = pb_begin();
      p = pb_push1(p, NV097_SET_SPECULAR_PARAMS + offset, 0x0EBAEBA0);
      pb_end(p);
    });

    snprintf(buf, sizeof(buf), "NV097_SET_SPECULAR_PARAMS_%d 0x0", i);
    ApplyAndLog(buf, [offset]() {
      auto p = pb_begin();
      p = pb_push1f(p, NV097_SET_SPECULAR_PARAMS + offset, 0.1234f);
      pb_end(p);
    });

    snprintf(buf, sizeof(buf), "NV097_SET_SPECULAR_PARAMS_%d 0xDEADBEEF", i);
    ApplyAndLog(buf, [offset]() {
      auto p = pb_begin();
      p = pb_push1(p, NV097_SET_SPECULAR_PARAMS + offset, 0xDEADBEEF);
      pb_end(p);
    });

    reset(offset);

    fprintf(log_stream_, "END NV097_SET_SPECULAR_PARAMS_%d\n", i);
  }

}
