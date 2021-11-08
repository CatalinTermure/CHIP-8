#ifndef CHIP8_SRC_RENDERING_PIPELINE_H_
#define CHIP8_SRC_RENDERING_PIPELINE_H_

#include <vulkan/vulkan.hpp>
#include "window.h"
#include "graphical_device.h"
#include "pipeline_attachments.h"

namespace CHIP8 {
class Pipeline {
 public:
  Pipeline(const Window &window,
           const GraphicalDevice &graphical_device,
           const PipelineAttachments &pipeline_attachments);

  ~Pipeline();

  [[nodiscard]] vk::Pipeline pipeline() const { return pipeline_; };

  Pipeline(const Pipeline &other) = delete;
  Pipeline &operator=(const Pipeline &other) = delete;

 private:
  vk::ShaderModule vertex_shader_;
  vk::ShaderModule fragment_shader_;
  vk::Pipeline pipeline_;

  const GraphicalDevice *graphical_device_;
};
}

#endif //CHIP8_SRC_RENDERING_PIPELINE_H_
