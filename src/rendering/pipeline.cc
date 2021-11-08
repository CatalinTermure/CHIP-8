#include "pipeline.h"

#include <fstream>
#include <filesystem>

#include <vulkan/vulkan.hpp>

static vk::ShaderModule CreateShaderModule(vk::Device device, const std::string &path);

CHIP8::Pipeline::Pipeline(const Window &window,
                          const GraphicalDevice &graphical_device,
                          const PipelineAttachments &pipeline_attachments) {
  graphical_device_ = &graphical_device;

  auto shader_folder_path = std::filesystem::current_path().parent_path().append("shaders");
  if (!std::filesystem::exists(shader_folder_path)) {
    assert(!"Could not find shader folder!");
  }

  vertex_shader_ =
      CreateShaderModule(graphical_device.device(), (shader_folder_path / "render_shader.vert.spv").string());
  fragment_shader_ =
      CreateShaderModule(graphical_device.device(), (shader_folder_path / "render_shader.frag.spv").string());

  // Pipeline creation

  vk::Extent2D window_extent{window.drawable_width(), window.drawable_height()};

  std::vector<vk::PipelineShaderStageCreateInfo> shader_stages = {
      vk::PipelineShaderStageCreateInfo{{}, vk::ShaderStageFlagBits::eVertex, vertex_shader_, "main", nullptr},
      vk::PipelineShaderStageCreateInfo{{}, vk::ShaderStageFlagBits::eFragment, fragment_shader_, "main", nullptr}
  };

  vk::PipelineInputAssemblyStateCreateInfo
      input_assembly_create_info{{}, vk::PrimitiveTopology::eTriangleList, false};

  vk::Viewport viewport{0.0f, 0.0f,
                        static_cast<float>(window_extent.width), static_cast<float>(window_extent.height),
                        0.0f, 1.0f};
  vk::Rect2D scissor{vk::Offset2D{0, 0}, window_extent};
  vk::PipelineViewportStateCreateInfo viewport_state_create_info{
      {},
      viewport,
      scissor
  };

  vk::PipelineRasterizationStateCreateInfo rasterization_state_create_info{
      {},
      static_cast<vk::Bool32>(0u),
      static_cast<vk::Bool32>(0u),
      vk::PolygonMode::eFill,
      vk::CullModeFlagBits::eBack,
      vk::FrontFace::eClockwise,
      static_cast<vk::Bool32>(0u),
      0.0f,
      0.0f,
      0.0f,
      1.0f
  };

  vk::PipelineColorBlendAttachmentState color_blend_attachment_state{
      static_cast<vk::Bool32>(false),
      vk::BlendFactor::eOne,
      vk::BlendFactor::eZero,
      vk::BlendOp::eAdd,
      vk::BlendFactor::eOne,
      vk::BlendFactor::eZero,
      vk::BlendOp::eAdd,
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
          vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
  };

  vk::PipelineMultisampleStateCreateInfo multisample_state_create_info{};
  vk::PipelineColorBlendStateCreateInfo color_blend_state_create_info{
      {},
      static_cast<vk::Bool32>(false),
      vk::LogicOp::eClear,
      1,
      &color_blend_attachment_state,
      std::array<float, 4>{0, 0, 0, 0}
  };

  vk::PipelineVertexInputStateCreateInfo vertex_input_state{
      {},
      nullptr,
      nullptr
  };

  vk::GraphicsPipelineCreateInfo create_info{
      {},
      shader_stages,
      &vertex_input_state,
      &input_assembly_create_info,
      nullptr,
      &viewport_state_create_info,
      &rasterization_state_create_info,
      &multisample_state_create_info,
      nullptr,
      &color_blend_state_create_info,
      nullptr,
      pipeline_attachments.pipeline_layout(),
      pipeline_attachments.render_pass(),
      0,
      nullptr,
      -1
  };

  vk::ResultValue<vk::Pipeline> result_value = graphical_device.device().createGraphicsPipeline(nullptr, create_info);
  if (result_value.result != vk::Result::eSuccess) {
    throw std::runtime_error(to_string(result_value.result));
  }

  pipeline_ = result_value.value;
}

CHIP8::Pipeline::~Pipeline() {
  graphical_device_->device().destroyPipeline(pipeline_);
  graphical_device_->device().destroyShaderModule(vertex_shader_);
  graphical_device_->device().destroyShaderModule(fragment_shader_);
}

vk::ShaderModule CreateShaderModule(vk::Device device, const std::string &path) {
  std::ifstream input = std::ifstream{path, std::ifstream::ate | std::ifstream::binary};
  size_t shader_code_size = input.tellg();
  std::vector<char> buffer(shader_code_size);
  input.seekg(0);
  input.read(buffer.data(), shader_code_size);

  return device.createShaderModule(vk::ShaderModuleCreateInfo{
      {},
      shader_code_size,
      reinterpret_cast<uint32_t *>(buffer.data())
  });
}