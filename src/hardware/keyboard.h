#ifndef CHIP8_SRC_HARDWARE_KEYBOARD_H_
#define CHIP8_SRC_HARDWARE_KEYBOARD_H_

/**
 * Keypad layout is:
 * 1 2 3 C
 * 4 5 6 D
 * 7 8 9 E
 * A 0 B F
 */

namespace CHIP8 {
class Keyboard {
 public:
  void HandleKey();
};
}

#endif //CHIP8_SRC_HARDWARE_KEYBOARD_H_
