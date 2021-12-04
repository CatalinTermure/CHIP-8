#ifndef CHIP8_SRC_HARDWARE_REGISTERS_H_
#define CHIP8_SRC_HARDWARE_REGISTERS_H_

#include <cstdint>

namespace CHIP8 {
class Registers {
 public:
  uint8_t v[16];
  uint16_t i;
};
}

#endif //CHIP8_SRC_HARDWARE_REGISTERS_H_
