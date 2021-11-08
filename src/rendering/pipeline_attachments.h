#ifndef CHIP8_SRC_RENDERING_PIPELINE_ATTACHMENTS_H_
#define CHIP8_SRC_RENDERING_PIPELINE_ATTACHMENTS_H_

#include <vector>

#include <vulkan/vulkan.hpp>
#include <glm/vec2.hpp>

#include "window.h"
#include "graphical_device.h"

namespace CHIP8 {
struct FSPushConstants {
  glm::vec2 window_size;
};

class PipelineAttachments {
 public:
  PipelineAttachments(const Window &window, const GraphicalDevice &graphical_device);
  ~PipelineAttachments();

  [[nodiscard]] vk::RenderPass render_pass() const { return render_pass_; };
  [[nodiscard]] vk::PipelineLayout pipeline_layout() const { return pipeline_layout_; };
  [[nodiscard]] vk::DescriptorSet descriptor_set(int index) const { return display_descriptor_sets_[index]; };

 private:
  vk::PipelineLayout pipeline_layout_;
  vk::RenderPass render_pass_;

  static constexpr uint32_t display_buffer_binding_slot_ = 0;
  vk::DescriptorSetLayout display_descriptor_set_layout_;
  std::vector<vk::DescriptorSet> display_descriptor_sets_;
  vk::DescriptorPool display_descriptor_pool_;

  const GraphicalDevice *graphical_device_;
};
}

#endif //CHIP8_SRC_RENDERING_PIPELINE_ATTACHMENTS_H_
