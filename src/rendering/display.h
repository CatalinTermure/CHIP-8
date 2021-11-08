#ifndef CHIP8_SRC_RENDERING_DISPLAY_H_
#define CHIP8_SRC_RENDERING_DISPLAY_H_

constexpr int kPixelWidth = 64;
constexpr int kPixelHeight = 32;

struct Display {
  bool pixels[kPixelWidth * kPixelHeight];
};

#endif //CHIP8_SRC_RENDERING_DISPLAY_H_
