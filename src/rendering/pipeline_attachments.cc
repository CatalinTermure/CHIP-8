#include "pipeline_attachments.h"

#include <vector>

#include <vulkan/vulkan.hpp>

// For future implementations where we have more than one swapchain image
// and more than one render thread.
static constexpr int kMaxDescriptorSets = 1;

static vk::RenderPass CreateRenderPass(vk::Device device, vk::Format image_format);

CHIP8::PipelineAttachments::PipelineAttachments(const CHIP8::Window &window,
                                                const CHIP8::GraphicalDevice &graphical_device) {
  graphical_device_ = &graphical_device;

  render_pass_ = CreateRenderPass(graphical_device.device(),
                                  window.GetSurfaceFormat(graphical_device.physical_device()).format);

  vk::DescriptorSetLayoutBinding display_buffer_binding{
      display_buffer_binding_slot_,
      vk::DescriptorType::eUniformBuffer,
      vk::ShaderStageFlagBits::eFragment,
      nullptr
  };

  display_descriptor_set_layout_ =
      graphical_device.device().createDescriptorSetLayout(vk::DescriptorSetLayoutCreateInfo{
          {},
          display_buffer_binding
      });

  vk::DescriptorPoolSize display_descriptor_size{vk::DescriptorType::eUniformBuffer, kMaxDescriptorSets};

  display_descriptor_pool_ = graphical_device.device().createDescriptorPool(vk::DescriptorPoolCreateInfo{
      {},
      kMaxDescriptorSets,
      display_descriptor_size
  });

  std::vector<vk::DescriptorSetLayout> descriptor_set_layouts(kMaxDescriptorSets, display_descriptor_set_layout_);

  display_descriptor_sets_ = graphical_device.device().allocateDescriptorSets(vk::DescriptorSetAllocateInfo{
    display_descriptor_pool_,
    descriptor_set_layouts
  });

  // TODO: bind descriptor sets to a memory buffer

  vk::PushConstantRange push_constant_range{vk::ShaderStageFlagBits::eFragment, 0, sizeof(FSPushConstants)};

  pipeline_layout_ = graphical_device.device().createPipelineLayout(vk::PipelineLayoutCreateInfo{
      {},
      display_descriptor_set_layout_,
      push_constant_range
  });
}

CHIP8::PipelineAttachments::~PipelineAttachments() {
  graphical_device_->device().destroyDescriptorPool(display_descriptor_pool_);

  graphical_device_->device().destroyPipelineLayout(pipeline_layout_);
  graphical_device_->device().destroyDescriptorSetLayout(display_descriptor_set_layout_);
  graphical_device_->device().destroyRenderPass(render_pass_);
}

vk::RenderPass CreateRenderPass(vk::Device device, vk::Format image_format) {
  vk::AttachmentDescription color_attachment_description{
      {},
      image_format,
      vk::SampleCountFlagBits::e1,
      vk::AttachmentLoadOp::eClear,
      vk::AttachmentStoreOp::eStore,
      vk::AttachmentLoadOp::eDontCare,
      vk::AttachmentStoreOp::eDontCare,
      vk::ImageLayout::eColorAttachmentOptimal,
      vk::ImageLayout::eColorAttachmentOptimal
  };

  vk::AttachmentReference color_attachment_reference{0, vk::ImageLayout::eColorAttachmentOptimal};

  vk::SubpassDescription color_subpass{
      {},
      vk::PipelineBindPoint::eGraphics,
      nullptr,
      color_attachment_reference,
      nullptr,
      nullptr,
      nullptr
  };

  return device.createRenderPass(vk::RenderPassCreateInfo{
      {},
      color_attachment_description,
      color_subpass,
      nullptr
  });
}
