#include "set_infinite_half_vector.h"

#include <pbkit/pbkit.h>

#   define NV097_SET_SPECULAR_PARAMS                          0x000009E0
#   define NV097_SET_SPECULAR_PARAMS_BACK                     0x00001E28

void SetInfiniteHalfVector::Run() {
  static constexpr float kVals[] = {
    1.0f, -1.0f, 0.12345f, 1024.f
  };

  char buf[64];

  for (uint32_t light = 0; light < 8; ++light) {
    auto command = SET_LIGHT(light, NV097_SET_LIGHT_INFINITE_HALF_VECTOR);

    for (uint32_t i = 0; i < 3; ++i) {
      fprintf(log_stream_, "BEGIN NV097_SET_LIGHT_INFINITE_HALF_VECTOR_%d_%d\n", light, i);

      auto offset = 4 * i;

      for (auto val: kVals) {
        uint32_t test_val = *(uint32_t *) &val;
        snprintf(buf, sizeof(buf), "NV097_SET_LIGHT_INFINITE_HALF_VECTOR_%d_%d 0x%X", light, i, test_val);
        ApplyAndLog(buf,
                    [command, offset, test_val]() {
                      auto p = pb_begin();
                      p = pb_push1(p, command + offset, test_val);
                      pb_end(p);
                    });
      }

      fprintf(log_stream_, "END NV097_SET_SPECULAR_PARAMS_%d\n", i);
    }
  }
}
