#ifndef CHIP8_SRC_HARDWARE_STACK_H_
#define CHIP8_SRC_HARDWARE_STACK_H_

#include <cstdint>

namespace CHIP8 {
constexpr int kStackSize = 1000;

class Stack {
 public:
  void push(uint16_t address);
  uint16_t pop();
 private:
  uint16_t data[kStackSize]{};
  uint16_t stack_counter_ = 0;
};
}

#endif //CHIP8_SRC_HARDWARE_STACK_H_
