#ifndef CHIP8_SRC_RENDERING_RENDER_CONTEXT_H_
#define CHIP8_SRC_RENDERING_RENDER_CONTEXT_H_

#include <vulkan/vulkan.hpp>

#include "pipeline_attachments.h"
#include "pipeline.h"
#include "swapchain.h"
#include "display.h"

namespace CHIP8 {
class RenderContext {
 public:
  RenderContext(const GraphicalDevice &graphical_device,
                const PipelineAttachments &pipeline_attachments,
                const Pipeline &pipeline,
                const Swapchain &swapchain);

  ~RenderContext();

  void Render();

  RenderContext(const RenderContext &other) = delete;
  RenderContext& operator=(const RenderContext &other) = delete;
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

#endif //CHIP8_SRC_RENDERING_RENDER_CONTEXT_H_
