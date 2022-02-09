#ifndef CHIP8_SRC_HARDWARE_DEVICE_H_
#define CHIP8_SRC_HARDWARE_DEVICE_H_

#include <memory>
#include <mutex>

#include "memory.h"
#include "registers.h"
#include "stack.h"
#include "keyboard.h"
#include "../rendering/display.h"

namespace CHIP8 {
class Device {
 public:
  Device();

  [[nodiscard]] Memory &memory() const { return *memory_; }
  [[nodiscard]] Registers &registers() const { return *registers_; }
  [[nodiscard]] Stack &call_stack() const { return *call_stack_; }
  [[nodiscard]] Keyboard &keyboard() const { return *keyboard_; }

  [[nodiscard]] bool RenderSprite(const char data[], int size, int start_x, int start_y);
  void ClearDisplay();

  const uint32_t kSdlRenderEventType;
 private:
  std::unique_ptr<Memory> memory_;
  std::unique_ptr<Registers> registers_;
  std::unique_ptr<Stack> call_stack_;
  std::unique_ptr<Display> display_;
  std::unique_ptr<Keyboard> keyboard_;

  std::mutex display_mutex_;
};
}

#endif //CHIP8_SRC_HARDWARE_DEVICE_H_
