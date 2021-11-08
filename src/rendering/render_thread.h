#ifndef CHIP8_SRC_RENDERING_RENDER_THREAD_H_
#define CHIP8_SRC_RENDERING_RENDER_THREAD_H_

#include <vulkan/vulkan.hpp>

#include "pipeline_attachments.h"
#include "pipeline.h"
#include "swapchain.h"
#include "display.h"

namespace CHIP8 {
class RenderThread {
 public:
  RenderThread(const GraphicalDevice &graphical_device,
               const PipelineAttachments &pipeline_attachments,
               const Pipeline &pipeline,
               const Swapchain &swapchain);

  ~RenderThread();

  void Render();

  RenderThread(const RenderThread &other) = delete;
  RenderThread& operator=(const RenderThread &other) = delete;
 private:
  vk::CommandPool command_pool_;
  vk::CommandBuffer command_buffer_;
  vk::Semaphore acquire_image_semaphore_;
  vk::Semaphore image_released_semaphore_;

  vk::Queue graphics_queue_;
  vk::Device logical_device_;
  const PipelineAttachments *pipeline_attachments_;
  const Pipeline *pipeline_;
  const Swapchain *swapchain_;
};
}

#endif //CHIP8_SRC_RENDERING_RENDER_THREAD_H_
