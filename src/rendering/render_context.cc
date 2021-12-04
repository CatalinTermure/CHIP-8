#include "render_context.h"

#include <vulkan/vulkan.hpp>

#include "swapchain.h"

CHIP8::RenderContext::RenderContext(const GraphicalDevice &graphical_device,
                                    const CHIP8::PipelineAttachments &pipeline_attachments,
                                    const CHIP8::Pipeline &pipeline,
                                    const CHIP8::Swapchain &swapchain) {
  logical_device_ = graphical_device.device();
  graphics_queue_ = graphical_device.graphics_queue();
  pipeline_attachments_ = &pipeline_attachments;
  pipeline_ = &pipeline;
  swapchain_ = &swapchain;

  command_pool_ =
      graphical_device.device().createCommandPool(vk::CommandPoolCreateInfo{vk::CommandPoolCreateFlagBits::eTransient,
                                                                            graphical_device.graphics_family_index()});

  vk::CommandBufferAllocateInfo command_buffer_alloc_info{command_pool_, vk::CommandBufferLevel::ePrimary, 1};

  command_buffer_ = graphical_device.device().allocateCommandBuffers(command_buffer_alloc_info)[0];

  acquire_image_semaphore_ = graphical_device.device().createSemaphore(vk::SemaphoreCreateInfo{});
  image_released_semaphore_ = graphical_device.device().createSemaphore(vk::SemaphoreCreateInfo{});
}

CHIP8::RenderContext::~RenderContext() {
  logical_device_.destroySemaphore(acquire_image_semaphore_);
  logical_device_.destroySemaphore(image_released_semaphore_);
  logical_device_.destroyCommandPool(command_pool_);
}

void CHIP8::RenderContext::Render() {
  vk::Extent2D swapchain_extent = swapchain_->image_extent();
  logical_device_.resetCommandPool(command_pool_);
  SwapchainImage swapchain_image = swapchain_->AcquireNextImage(acquire_image_semaphore_);

  command_buffer_.begin(vk::CommandBufferBeginInfo{vk::CommandBufferUsageFlagBits::eOneTimeSubmit, nullptr});

  vk::ImageMemoryBarrier render_begin_barrier = vk::ImageMemoryBarrier{
      vk::AccessFlagBits::eNoneKHR,
      vk::AccessFlagBits::eColorAttachmentWrite,
      vk::ImageLayout::eUndefined,
      vk::ImageLayout::eColorAttachmentOptimal,
      0, 0,
      swapchain_image.image,
      vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, VK_REMAINING_MIP_LEVELS, 0,
                                VK_REMAINING_ARRAY_LAYERS,}
  };

  command_buffer_.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                  vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                  vk::DependencyFlagBits::eByRegion,
                                  nullptr,
                                  nullptr,
                                  render_begin_barrier);

  vk::ClearValue clear_value{vk::ClearColorValue{std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}}};

  command_buffer_.beginRenderPass(vk::RenderPassBeginInfo{
      pipeline_attachments_->render_pass(),
      swapchain_image.framebuffer,
      vk::Rect2D{vk::Offset2D{0, 0}, swapchain_extent},
      clear_value
  }, vk::SubpassContents::eInline);

  command_buffer_.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline_->pipeline());
/*
  command_buffer_.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                     pipeline_attachments_->pipeline_layout(),
                                     0,
                                     pipeline_attachments_->descriptor_set(static_cast<int>(swapchain_image.index)),
                                     nullptr);
*/
  FSPushConstants fragment_shader_push_constants{
      .window_size = glm::vec2{swapchain_extent.width, swapchain_extent.height}
  };

  command_buffer_.pushConstants(pipeline_attachments_->pipeline_layout(),
                                vk::ShaderStageFlagBits::eFragment,
                                0u,
                                sizeof(FSPushConstants),
                                (void *) &fragment_shader_push_constants);

  command_buffer_.draw(6, 1, 0, 0);

  command_buffer_.endRenderPass();

  vk::ImageMemoryBarrier present_barrier = vk::ImageMemoryBarrier{
      vk::AccessFlagBits::eColorAttachmentWrite,
      vk::AccessFlagBits::eNoneKHR,
      vk::ImageLayout::eColorAttachmentOptimal,
      vk::ImageLayout::ePresentSrcKHR,
      0, 0,
      swapchain_image.image,
      vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, VK_REMAINING_MIP_LEVELS, 0,
                                VK_REMAINING_ARRAY_LAYERS,}
  };

  command_buffer_.pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                  vk::PipelineStageFlagBits::eTopOfPipe,
                                  vk::DependencyFlagBits::eByRegion,
                                  nullptr,
                                  nullptr,
                                  present_barrier);

  command_buffer_.end();

  vk::PipelineStageFlags submit_stage_flags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
  vk::SubmitInfo submit_info{acquire_image_semaphore_, submit_stage_flags, command_buffer_, image_released_semaphore_};
  graphics_queue_.submit(submit_info, nullptr);

  vk::SwapchainKHR swapchain = swapchain_->swapchain();
  vk::Result result =
      graphics_queue_.presentKHR(vk::PresentInfoKHR{image_released_semaphore_, swapchain, swapchain_image.index,
                                                    nullptr});

  logical_device_.waitIdle();
}
