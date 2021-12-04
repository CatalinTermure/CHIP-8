#include "program.h"

#include <fstream>

uint16_t CHIP8::Program::GetNextInstruction() {
  if (program_counter_ >= instructions_.size()) {
    return kEndInstruction;
  }
  return instructions_[program_counter_++];
}

CHIP8::Program::Program(const std::filesystem::path &file_path) {
  program_counter_ = 0;
  
  std::ifstream inf(file_path.string(), std::ios::in | std::ios::binary | std::ios::ate);
  instructions_.resize(inf.tellg() / 2);
  inf.seekg(0, std::ios::beg);
  std::string buffer;
  buffer.resize(instructions_.size() * 2);
  inf.read(buffer.data(), instructions_.size() * 2);
  inf.close();

  for (int i = 0; i < instructions_.size(); i++) {
    instructions_[i] = static_cast<uint16_t>(buffer[2 * i]) * 0x100 +
        static_cast<uint16_t>(buffer[2 * i + 1]);
  }
}
