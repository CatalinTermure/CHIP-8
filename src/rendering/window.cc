#include "window.h"

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include "../debug_messenger.h"

static constexpr bool kUseCustomDebugMessenger = false;
static constexpr bool kIncludeValidationLayers = true;
static constexpr const char kEngineName[] = "ChovEngine";

static constexpr vk::Format kPreferredImageFormat = vk::Format::eB8G8R8A8Srgb;
static constexpr vk::ColorSpaceKHR kPreferredColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;

static vk::Instance CreateInstance(SDL_Window *window, const char *title);
static vk::SurfaceKHR CreateSurface(SDL_Window *window, vk::Instance instance);

CHIP8::Window::~Window() {
  instance_.destroySurfaceKHR(surface_);
  instance_.destroy();
  SDL_DestroyWindow(window_);
}

CHIP8::Window::Window(const char *title, int width, int height) {
  window_ = SDL_CreateWindow(title,
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             width,
                             height,
                             SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN
                                 | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

  pixel_width_ = width;
  pixel_height_ = height;

  SDL_Vulkan_GetDrawableSize(window_, &width, &height);
  drawable_width_ = width;
  drawable_height_ = height;

  instance_ = CreateInstance(window_, title);
  if (kUseCustomDebugMessenger) SetupDebugMessenger(instance_);

  surface_ = CreateSurface(window_, instance_);
}

vk::SurfaceKHR CreateSurface(SDL_Window *window, vk::Instance instance) {
  VkSurfaceKHR surface_raw;
  if (!SDL_Vulkan_CreateSurface(window, static_cast<VkInstance>(instance), &surface_raw)) {
    throw std::runtime_error("Could not create surface_raw for SDL Window");
  }
  return vk::SurfaceKHR{surface_raw};
}

vk::Instance CreateInstance(SDL_Window *window, const char *title) {
  vk::ApplicationInfo app_info{title,
                               VK_MAKE_VERSION(0, 0, 0),
                               kEngineName,
                               VK_MAKE_VERSION(0, 0, 0),
                               VK_API_VERSION_1_1};

  std::vector<const char *> validation_layers{};

  if (kIncludeValidationLayers) {
    validation_layers.push_back("VK_LAYER_KHRONOS_validation");
  }

  unsigned int sdl_extension_count = 0;
  SDL_Vulkan_GetInstanceExtensions(window, &sdl_extension_count, nullptr);
  std::vector<const char *> instance_extensions{sdl_extension_count};
  SDL_Vulkan_GetInstanceExtensions(window, &sdl_extension_count, instance_extensions.data());

  instance_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
  instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  vk::InstanceCreateInfo instance_create_info{vk::InstanceCreateFlags{}, &app_info,
                                              validation_layers,
                                              instance_extensions};

  vk::Instance instance = vk::createInstance(instance_create_info);

  return instance;
}

vk::SurfaceFormatKHR CHIP8::Window::GetSurfaceFormat(vk::PhysicalDevice physical_device) const {
  std::vector<vk::SurfaceFormatKHR> surface_formats = physical_device.getSurfaceFormatsKHR(surface_);
  vk::SurfaceFormatKHR result = surface_formats[0];
  for (const auto &surface_format: surface_formats) {
    if (surface_format.format == kPreferredImageFormat && surface_format.colorSpace == kPreferredColorSpace) {
      result = surface_format;
    }
  }

  return vk::SurfaceFormatKHR{result};
}