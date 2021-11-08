#ifndef CHIP8_SRC_RENDERING_SWAPCHAIN_H_
#define CHIP8_SRC_RENDERING_SWAPCHAIN_H_

#include <vector>

#include <vulkan/vulkan.hpp>

#include "window.h"
#include "graphical_device.h"
#include "pipeline_attachments.h"

namespace CHIP8 {
struct SwapchainImage {
  unsigned int index;
  vk::Image image;
  vk::ImageView image_view;
  vk::Framebuffer framebuffer;
};

class Swapchain {
 public:
  Swapchain(const Window &window,
            const GraphicalDevice &graphical_device,
            const PipelineAttachments &pipeline_attachments);
  ~Swapchain();

  [[nodiscard]] SwapchainImage AcquireNextImage(vk::Semaphore signal_semaphore) const;

  [[nodiscard]] vk::SwapchainKHR swapchain() const { return swapchain_; };
  [[nodiscard]] vk::Extent2D image_extent() const { return image_extent_; };

  Swapchain(const Swapchain &other) = delete;
  Swapchain &operator=(const Swapchain &other) = delete;

 private:
  vk::SwapchainKHR swapchain_;
  vk::Extent2D image_extent_;
  size_t image_count_;
  vk::Format image_format_;
  std::vector<vk::Framebuffer> framebuffers_;
  std::vector<vk::Image> images_;
  std::vector<vk::ImageView> image_views_;
  const GraphicalDevice *graphical_device_;
};
}

#endif //CHIP8_SRC_RENDERING_SWAPCHAIN_H_
