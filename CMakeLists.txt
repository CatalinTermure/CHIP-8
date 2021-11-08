cmake_minimum_required(VERSION 3.20)
project(CHIP8)

set(CMAKE_CXX_STANDARD 20)
set(SDL2_DIR "C:/SDL/SDL2-2.0.16")
set(GLSLC_EXECUTABLE "glslc.exe")

find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)

set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADER_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

file(GLOB SHADERS
        ${SHADER_SOURCE_DIR}/*.vert
        ${SHADER_SOURCE_DIR}/*.frag)

foreach (source IN LISTS SHADERS)
    get_filename_component(FILENAME ${source} NAME)
    add_custom_command(
            COMMAND ${GLSLC_EXECUTABLE} -o ${SHADER_BINARY_DIR}/${FILENAME}.spv ${source}
            OUTPUT ${SHADER_BINARY_DIR}/${FILENAME}.spv
            DEPENDS ${source} ${SHADER_BINARY_DIR}
    )
    list(APPEND SPV_SHADERS ${SHADER_BINARY_DIR}/${FILENAME}.spv)
endforeach ()

add_custom_target(shaders ALL DEPENDS ${SPV_SHADERS})

add_executable(CHIP8 src/main.cc src/debug_messenger.h src/rendering/graphical_device.cc src/rendering/graphical_device.h src/rendering/window.cc src/rendering/window.h src/rendering/swapchain.cc src/rendering/swapchain.h src/rendering/pipeline_attachments.cc src/rendering/pipeline_attachments.h src/rendering/pipeline.cc src/rendering/pipeline.h src/rendering/render_thread.cc src/rendering/render_thread.h src/rendering/display.h)

target_include_directories(
        CHIP8
        PUBLIC include
        PUBLIC ${Vulkan_INCLUDE_DIRS}
        PUBLIC ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(
        CHIP8
        PUBLIC ${Vulkan_LIBRARIES}
        PUBLIC ${SDL2_LIBRARIES}
)