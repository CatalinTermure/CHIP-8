# CHIP-8 Interpreter

This is a simple CHIP-8 Interpreter written in C++20, with Vulkan 1.1-based rendering.
The CHIP-8 specification is taken [from here](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM).

This project was made on Windows, but the code should be cross-platform with minimal tweaks.

This project was made with the goal of getting familiar with Vulkan, CPU and GPU architecture.

# Status & usage

The project is in a WIP state, as described below:
* Rendering - mostly done
* Instruction reading and memory model - done
* Instruction execution <------ 50% of instructions done
* Sound (tentative)

Currently, you cannot interact with the app in any way.

# Dependencies

* [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/)
* [SDL 2.0.16](https://www.libsdl.org/download-2.0.php)
* [GLSLC](https://github.com/google/shaderc) (this is also bundled with the Vulkan SDK)
* [CMake](https://cmake.org/download/)

# Building the project

Check the CMakeLists.txt and
make sure SDL2_DIR points to the directory where you installed SDL2.0.16
and GLSLC_EXECUTABLE points to the path of your glslc executable,
or to "glslc.exe" if it is added to your PATH environmental variable.

After this, building should be as straight forward as using CMake.

# Contributions

This project uses [Google's C++ Code Style Guide](https://google.github.io/styleguide/cppguide.html),
with the caveat that lines may be up to 120 characters long because vulkan tends to be extremely verbose.

# License

You are free to use my code however you want, while respecting the license of 3rd party libraries. Attribution to me would be nice, but not mandatory