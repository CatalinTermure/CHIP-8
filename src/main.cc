#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_vulkan.h>
#undef main

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <optional>
#include <fstream>

#include <vulkan/vulkan.hpp>

#include "rendering/window.h"
#include "rendering/graphical_device.h"
#include "rendering/pipeline_attachments.h"
#include "rendering/swapchain.h"
#include "rendering/pipeline.h"
#include "rendering/render_thread.h"

const int kWindowWidth = 1080;
const int kWindowHeight = 800;
constexpr const char kAppName[] = "CHIP-8 Interpreter";

int main(int, char **) {
  CHIP8::Window window{kAppName, kWindowWidth, kWindowHeight};
  CHIP8::GraphicalDevice graphical_device{window};
  CHIP8::PipelineAttachments pipeline_attachments{window, graphical_device};
  CHIP8::Swapchain swapchain{window, graphical_device, pipeline_attachments};
  CHIP8::Pipeline pipeline{window, graphical_device, pipeline_attachments};
  CHIP8::RenderThread render_thread{graphical_device, pipeline_attachments, pipeline, swapchain};

  SDL_Event sdl_event;

  bool should_window_close = false;

  while (!should_window_close) {
    while (SDL_PollEvent(&sdl_event)) {
      switch (sdl_event.type) {
        case SDL_QUIT: should_window_close = true;
          break;
      }
    }

    if (should_window_close) break;

    render_thread.Render();
  }
  return 0;
}