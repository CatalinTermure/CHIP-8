#include "device.h"

#include <cstring>
#include <mutex>
#include <SDL_events.h>

void CHIP8::Device::ClearDisplay() {
  const std::lock_guard<std::mutex> kLock{display_mutex_};
  memset(display_->pixels, 0, kDisplayPixelColumns * kDisplayPixelRows * sizeof(bool));
}

bool CHIP8::Device::RenderSprite(const char data[], int size, int start_x, int start_y) {
  const std::lock_guard<std::mutex> kLock{display_mutex_};
  bool collision = false;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < 8; j++) {
      int x = (start_x + j) % kDisplayPixelColumns;
      int y = (start_y + i) % kDisplayPixelRows;
      if (display_->pixels[y * kDisplayPixelColumns + x]) collision = true;
      display_->pixels[y * kDisplayPixelColumns + x] ^= ((data[i] & (1 << j)) >> j);
    }
  }

  SDL_Event sdl_event;
  sdl_event.type = kSdlRenderEventType;
  SDL_PushEvent(&sdl_event); // event is copied into the queue, so we don't need to worry

  return collision;
}
CHIP8::Device::Device() : kSdlRenderEventType(SDL_RegisterEvents(1)) {
  memory_ = std::make_unique<Memory>();
  registers_ = std::make_unique<Registers>();
  call_stack_ = std::make_unique<Stack>();
  display_ = std::make_unique<Display>();
  keyboard_ = std::make_unique<Keyboard>();
}
