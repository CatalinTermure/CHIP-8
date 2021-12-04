#ifndef CHIP8_SRC_HARDWARE_PROGRAM_H_
#define CHIP8_SRC_HARDWARE_PROGRAM_H_

#include <vector>
#include <filesystem>
namespace CHIP8 {
class Program {
 public:
  constexpr static uint16_t kEndInstruction = 0xFFFF;

  explicit Program(const std::filesystem::path& file_path);

  void Jump(uint16_t address) { program_counter_ = address; }
  uint16_t GetNextInstruction();

  Program(Program &other) = delete;
  Program &operator=(Program &other) = delete;

 private:
  uint16_t program_counter_;
  std::vector<uint16_t> instructions_;
};
}

#endif //CHIP8_SRC_HARDWARE_PROGRAM_H_
