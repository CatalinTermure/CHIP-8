#ifndef CHIP8_SRC_RENDERING_DISPLAY_H_
#define CHIP8_SRC_RENDERING_DISPLAY_H_

namespace CHIP8 {
constexpr int kDisplayPixelColumns = 64;
constexpr int kDisplayPixelRows = 32;

struct Display {
  bool pixels[kDisplayPixelColumns * kDisplayPixelRows];
};
}

#endif //CHIP8_SRC_RENDERING_DISPLAY_H_
