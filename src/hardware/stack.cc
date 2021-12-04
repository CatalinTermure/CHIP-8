#include "stack.h"

uint16_t CHIP8::Stack::pop() {
  if (stack_counter_ == 0) return -1;

  return data[--stack_counter_];
}

void CHIP8::Stack::push(uint16_t address) {
  data[stack_counter_++] = address;
}
