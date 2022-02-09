#include "keyboard.h"

void CHIP8::Keyboard::HandleKeyDown(SDL_KeyCode sdl_key_code) {
  if (!keymaps_.contains(sdl_key_code)) return;
  const std::lock_guard<std::mutex> kLock{kb_mutex_};
  key_states_[(int) keymaps_[sdl_key_code]] = true;
}
void CHIP8::Keyboard::HandleKeyUp(SDL_KeyCode sdl_key_code) {
  if (!keymaps_.contains(sdl_key_code)) return;
  const std::lock_guard<std::mutex> kLock{kb_mutex_};
  key_states_[(int) keymaps_[sdl_key_code]] = false;
}
bool CHIP8::Keyboard::IsPressed(int key_index) {
  const std::lock_guard<std::mutex> kLock{kb_mutex_};
  return key_states_[(int) keycodes_[key_index]];
}
