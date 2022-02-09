#ifndef CHIP8_SRC_HARDWARE_KEYBOARD_H_
#define CHIP8_SRC_HARDWARE_KEYBOARD_H_

#include <SDL_keycode.h>

#include <unordered_map>
#include <mutex>
#include <array>

namespace CHIP8 {
enum class KeyCode {
  kKey1 = 0, kKey2, kKey3, kKeyC,
  kKey4, kKey5, kKey6, kKeyD,
  kKey7, kKey8, kKey9, kKeyE,
  kKeyA, kKey0, kKeyB, kKeyF,
  kNumKeyCodes
};

class Keyboard {
 public:
  void HandleKeyDown(SDL_KeyCode sdl_key_code);
  void HandleKeyUp(SDL_KeyCode sdl_key_code);
  bool IsPressed(int key_index);
 private:
  std::mutex kb_mutex_;
  bool key_states_[(int) KeyCode::kNumKeyCodes];

  std::array<KeyCode, 16> keycodes_ = {
      KeyCode::kKey0, KeyCode::kKey1, KeyCode::kKey2, KeyCode::kKey3,
      KeyCode::kKey4, KeyCode::kKey5, KeyCode::kKey6, KeyCode::kKey7,
      KeyCode::kKey8, KeyCode::kKey9, KeyCode::kKeyA, KeyCode::kKeyB,
      KeyCode::kKeyC, KeyCode::kKeyD, KeyCode::kKeyE, KeyCode::kKeyF,
  };

  std::unordered_map<SDL_KeyCode, KeyCode> keymaps_ = {
      {SDL_KeyCode::SDLK_1, KeyCode::kKey1},
      {SDL_KeyCode::SDLK_2, KeyCode::kKey2},
      {SDL_KeyCode::SDLK_3, KeyCode::kKey3},
      {SDL_KeyCode::SDLK_4, KeyCode::kKeyC},
      {SDL_KeyCode::SDLK_q, KeyCode::kKey4},
      {SDL_KeyCode::SDLK_w, KeyCode::kKey5},
      {SDL_KeyCode::SDLK_e, KeyCode::kKey6},
      {SDL_KeyCode::SDLK_r, KeyCode::kKeyD},
      {SDL_KeyCode::SDLK_a, KeyCode::kKey7},
      {SDL_KeyCode::SDLK_s, KeyCode::kKey8},
      {SDL_KeyCode::SDLK_d, KeyCode::kKey9},
      {SDL_KeyCode::SDLK_f, KeyCode::kKeyE},
      {SDL_KeyCode::SDLK_z, KeyCode::kKeyA},
      {SDL_KeyCode::SDLK_x, KeyCode::kKey0},
      {SDL_KeyCode::SDLK_c, KeyCode::kKeyB},
      {SDL_KeyCode::SDLK_v, KeyCode::kKeyF}
  };
};
}

#endif //CHIP8_SRC_HARDWARE_KEYBOARD_H_
