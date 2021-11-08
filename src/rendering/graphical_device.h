#ifndef CHIP8_SRC_RENDERING_GRAPHICAL_DEVICE_H_
#define CHIP8_SRC_RENDERING_GRAPHICAL_DEVICE_H_

#include <vulkan/vulkan.hpp>

#include "window.h"

namespace CHIP8 {
class GraphicalDevice {
 public:
  explicit GraphicalDevice(const Window &window);
  ~GraphicalDevice();

  [[nodiscard]] vk::PhysicalDevice physical_device() const { return physical_device_; };
  [[nodiscard]] vk::Device device() const { return logical_device_; };
  [[nodiscard]] vk::Queue graphics_queue() const { return graphics_queue_; }
  [[nodiscard]] uint32_t graphics_family_index() const { return graphics_family_index_; };

  GraphicalDevice(const GraphicalDevice &) = delete;
  GraphicalDevice& operator=(const GraphicalDevice &) = delete;
 private:
  vk::PhysicalDevice physical_device_;
  vk::Device logical_device_;
  uint32_t graphics_family_index_;
  vk::Queue graphics_queue_;
};
}

#endif // CHIP8_SRC_RENDERING_GRAPHICAL_DEVICE_H_
