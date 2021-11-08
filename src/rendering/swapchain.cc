#include "swapchain.h"

#include <vector>

#include <vulkan/vulkan.hpp>

static vk::SwapchainKHR CreateSwapchain(const CHIP8::Window &, const CHIP8::GraphicalDevice &, vk::SurfaceFormatKHR);

CHIP8::Swapchain::~Swapchain() {
  for (size_t i = 0; i < image_count_; i++) {
    graphical_device_->device().destroyFramebuffer(framebuffers_[i]);
    graphical_device_->device().destroyImageView(image_views_[i]);
  }
  graphical_device_->device().destroySwapchainKHR(swapchain_);
}

CHIP8::Swapchain::Swapchain(const CHIP8::Window &window,
                            const CHIP8::GraphicalDevice &graphical_device,
                            const PipelineAttachments &pipeline_attachments) {
  graphical_device_ = &graphical_device;
  image_extent_ = vk::Extent2D{window.drawable_width(), window.drawable_height()};

  vk::SurfaceFormatKHR surface_format = window.GetSurfaceFormat(graphical_device.physical_device());
  image_format_ = surface_format.format;
  swapchain_ = CreateSwapchain(window, graphical_device, surface_format);

  images_ = graphical_device.device().getSwapchainImagesKHR(swapchain_);
  image_count_ = images_.size();

  image_views_ = std::vector<vk::ImageView>(image_count_);
  framebuffers_ = std::vector<vk::Framebuffer>(image_count_);
  for (size_t i = 0; i < image_count_; i++) {
    image_views_[i] = graphical_device.device().createImageView(vk::ImageViewCreateInfo{
        {},
        images_[i],
        vk::ImageViewType::e2D,
        image_format_,
        vk::ComponentMapping{},
        vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}
    });

    framebuffers_[i] = graphical_device.device().createFramebuffer(vk::FramebufferCreateInfo{
        {},
        pipeline_attachments.render_pass(),
        image_views_[i],
        window.drawable_width(),
        window.drawable_height(),
        1
    });
  }
}

CHIP8::SwapchainImage CHIP8::Swapchain::AcquireNextImage(vk::Semaphore signal_semaphore) const {
  unsigned int image_index =
      graphical_device_->device().acquireNextImageKHR(swapchain_, UINT64_MAX, signal_semaphore, nullptr).value;

  return SwapchainImage{
      .index = image_index,
      .image = images_[image_index],
      .image_view = image_views_[image_index],
      .framebuffer = framebuffers_[image_index]
  };
}

vk::SwapchainKHR CreateSwapchain(const CHIP8::Window &window,
                                 const CHIP8::GraphicalDevice &graphical_device,
                                 vk::SurfaceFormatKHR surface_format) {
  vk::SurfaceCapabilitiesKHR
      capabilities = graphical_device.physical_device().getSurfaceCapabilitiesKHR(window.surface());

  if (!(capabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)) {
    throw std::runtime_error("Device does not support displaying without a surface transform");
  }

  vk::Extent2D swapchain_extent = capabilities.currentExtent;
  if (swapchain_extent.width == UINT32_MAX || swapchain_extent.height == UINT32_MAX) {
    swapchain_extent = vk::Extent2D{window.drawable_width(), window.drawable_height()};
  }

  vk::SwapchainCreateInfoKHR create_info{
      {},
      window.surface(),
      std::clamp(1u, capabilities.minImageCount, capabilities.maxImageCount),
      surface_format.format,
      surface_format.colorSpace,
      swapchain_extent,
      1,
      vk::ImageUsageFlagBits::eColorAttachment,
      vk::SharingMode::eExclusive,
      0,
      nullptr,
      vk::SurfaceTransformFlagBitsKHR::eIdentity,
      vk::CompositeAlphaFlagBitsKHR::eOpaque,
      vk::PresentModeKHR::eFifo,
      (uint32_t) true,
      nullptr
  };

  return graphical_device.device().createSwapchainKHR(create_info);
}