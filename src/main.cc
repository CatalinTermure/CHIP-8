#define SDL_MAIN_HANDLED
#include <SDL.h>
#undef main

#include <filesystem>
#include <iostream>

#include "rendering/window.h"
#include "rendering/graphical_device.h"
#include "rendering/pipeline_attachments.h"
#include "rendering/swapchain.h"
#include "rendering/pipeline.h"
#include "rendering/render_context.h"
#include "hardware/device.h"
#include "instructions/program.h"
#include "instructions/execution_thread.h"

constexpr int kWindowWidth = 1080;
constexpr int kWindowHeight = 800;
constexpr char kAppName[] = "CHIP-8 Interpreter";

int main(int, char **) {
  CHIP8::Window window{kAppName, kWindowWidth, kWindowHeight};
  CHIP8::GraphicalDevice graphical_device{window};
  CHIP8::PipelineAttachments pipeline_attachments{window, graphical_device};
  CHIP8::Swapchain swapchain{window, graphical_device, pipeline_attachments};
  CHIP8::Pipeline pipeline{window, graphical_device, pipeline_attachments};
  CHIP8::RenderContext render_context{graphical_device, pipeline_attachments, pipeline, swapchain};

  CHIP8::Device device{};
  CHIP8::Program
      testing_program{std::filesystem::current_path().parent_path() / "programs" / "testing" / "register-loading.ch8"};

  CHIP8::ExecutionThread{device, testing_program, false};

  SDL_Event sdl_event;

  bool should_window_close = false;

  while (!should_window_close) {
    if (SDL_WaitEvent(&sdl_event)) {
      switch (sdl_event.type) {
        case SDL_QUIT: should_window_close = true;
          break;
        case SDL_KEYDOWN:
          device.keyboard().HandleKeyDown(static_cast<SDL_KeyCode>(sdl_event.key.keysym.sym));
          break;
        case SDL_KEYUP:
          device.keyboard().HandleKeyUp(static_cast<SDL_KeyCode>(sdl_event.key.keysym.sym));
          break;
      }
    }

    render_context.Render();
  }
  return 0;
}