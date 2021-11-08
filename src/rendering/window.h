#ifndef CHIP8_SRC_RENDERING_WINDOW_H_
#define CHIP8_SRC_RENDERING_WINDOW_H_

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

namespace CHIP8 {
class Window {
 public:
  [[nodiscard]] vk::SurfaceFormatKHR GetSurfaceFormat(vk::PhysicalDevice physical_device) const;

  [[nodiscard]] vk::Instance instance() const { return instance_; };
  [[nodiscard]] vk::SurfaceKHR surface() const { return surface_; };

  [[nodiscard]] uint32_t width() const { return pixel_width_; };
  [[nodiscard]] uint32_t height() const { return pixel_height_; };

  // drawable width and height are used for drawing on high DPI displays
  // where pixel size and drawable size are not equal
  [[nodiscard]] uint32_t drawable_width() const { return drawable_width_; };
  [[nodiscard]] uint32_t drawable_height() const { return drawable_height_; };

  Window(const char *title, int width, int height);
  ~Window();

  Window() = delete;
  Window(const Window &other) = delete;
  Window &operator=(const Window &other) = delete;

 private:
  SDL_Window *window_;
  vk::Instance instance_;
  vk::SurfaceKHR surface_;
  uint32_t pixel_width_;
  uint32_t pixel_height_;
  uint32_t drawable_width_;
  uint32_t drawable_height_;
};
}

#endif //CHIP8_SRC_RENDERING_WINDOW_H_
