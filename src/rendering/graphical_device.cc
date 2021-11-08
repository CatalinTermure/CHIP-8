#include "graphical_device.h"

#include <algorithm>

#include <vulkan/vulkan.hpp>

vk::QueueFlags required_queues = vk::QueueFlags{vk::QueueFlagBits::eGraphics};

static int GetDeviceSuitability(vk::PhysicalDevice physical_device);
static vk::PhysicalDevice PickPhysicalDevice(vk::Instance instance);
static uint32_t GetGraphicsQueueIndex(vk::PhysicalDevice physical_device, vk::SurfaceKHR surface);
static vk::Device CreateLogicalDevice(vk::PhysicalDevice physical_device, uint32_t graphics_family_index);

CHIP8::GraphicalDevice::~GraphicalDevice() {
  logical_device_.destroy();
}

CHIP8::GraphicalDevice::GraphicalDevice(const CHIP8::Window &window) {
  physical_device_ = PickPhysicalDevice(window.instance());
  graphics_family_index_ = GetGraphicsQueueIndex(physical_device_, window.surface());
  logical_device_ = CreateLogicalDevice(physical_device_, graphics_family_index_);
  graphics_queue_ = logical_device_.getQueue(graphics_family_index_, 0);
}

int GetDeviceSuitability(vk::PhysicalDevice physical_device) {
  int suitability = 0;
  std::vector<vk::QueueFamilyProperties> queue_families_props = physical_device.getQueueFamilyProperties();

  vk::QueueFlags available_queue_families{};
  for (auto queue_family_props: queue_families_props) available_queue_families |= queue_family_props.queueFlags;

  if (!(available_queue_families & required_queues)) return -1;

  // We prefer integrated GPUs over dedicated GPUs
  // because there's no need to transfer data over the PCIe bus
  // and the rendering work is simple enough that we can afford it.
  if (physical_device.getProperties().deviceType == vk::PhysicalDeviceType::eIntegratedGpu) {
    suitability += 8;
  }

  return suitability;
}

vk::PhysicalDevice PickPhysicalDevice(vk::Instance instance) {
  std::vector<vk::PhysicalDevice> physical_devices = instance.enumeratePhysicalDevices();
  std::vector<int> suitabilities(physical_devices.size());
  for (int i = 0; i < physical_devices.size(); i++) {
    suitabilities[i] = GetDeviceSuitability(physical_devices[i]);
  }
  int best_device_index = std::max_element(suitabilities.begin(), suitabilities.end()) - suitabilities.begin();
  if (suitabilities[best_device_index] == -1) {
    throw std::runtime_error("No available device can run this software.");
  }

  return physical_devices[best_device_index];
}

uint32_t GetGraphicsQueueIndex(vk::PhysicalDevice physical_device, vk::SurfaceKHR surface) {
  uint32_t result;
  std::vector<vk::QueueFamilyProperties> queue_families_props = physical_device.getQueueFamilyProperties();
  bool found_graphics_family = false;
  for (int i = 0; i < queue_families_props.size(); i++) {
    if ((queue_families_props[i].queueFlags & vk::QueueFlagBits::eGraphics)
        && physical_device.getSurfaceSupportKHR(i, surface)) {
      found_graphics_family = true;
      result = i;
    }
  }

  if (!found_graphics_family) {
    throw std::runtime_error("Could not find a graphics family");
  }

  return result;
}

vk::Device CreateLogicalDevice(vk::PhysicalDevice physical_device, uint32_t graphics_family_index) {
  std::vector<vk::DeviceQueueCreateInfo> queue_create_infos{};
  std::array<float, 1> graphics_priorities = {1.0f};
  vk::DeviceQueueCreateInfo graphics_queue_create_info{{},
                                                       graphics_family_index,
                                                       graphics_priorities};
  queue_create_infos.push_back(graphics_queue_create_info);

  std::array<const char *const, 0> enabled_layers{};
  std::array<const char *const, 1> enabled_extensions{
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };

  vk::DeviceCreateInfo
      device_create_info{vk::DeviceCreateFlags(), queue_create_infos, enabled_layers, enabled_extensions};

  return physical_device.createDevice(device_create_info);
}