#ifndef CHIP8_SRC_HARDWARE_MEMORY_H_
#define CHIP8_SRC_HARDWARE_MEMORY_H_

namespace CHIP8 {
struct Memory {
  char data[0xFFF + 1];
};
}
#endif //CHIP8_SRC_HARDWARE_MEMORY_H_
