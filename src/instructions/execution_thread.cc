#include "execution_thread.h"

#include <cstdlib>

static void ExecuteProgram(CHIP8::Device &device, CHIP8::Program &program) {
  uint16_t instruction = program.GetNextInstruction();
  std::srand(std::time(nullptr));
  while (instruction != CHIP8::Program::kEndInstruction) {
    int register_x_index = (instruction & 0x0F00) >> 8;
    int register_y_index = (instruction & 0x00F0) >> 4;
    if (instruction == 0x00E0) { // CLS
      device.ClearDisplay();
    } else if (instruction == 0x00EE) { // RET
      program.Jump(device.call_stack().pop());
    } else if (!(instruction & ~0x0FFF)) { // SYS addr
      throw std::runtime_error("This interpreter does not support SYS addr instructions.");
    } else if ((instruction & 0x1FFF) == instruction) { // JP addr
      program.Jump(instruction & 0x0FFF);
    } else if ((instruction & 0x2FFF) == instruction) { // CALL addr
      device.call_stack().push(instruction & 0x0FFF);
    } else if ((instruction & 0x3FFF) == instruction) { // SE Vx, byte
      if (device.registers().v[register_x_index] == (instruction & 0x00FF)) {
        program.GetNextInstruction();
      }
    } else if ((instruction & 0x4FFF) == instruction) { // SNE Vx, byte
      if (device.registers().v[register_x_index] != (instruction & 0x00FF)) {
        program.GetNextInstruction();
      }
    } else if ((instruction & 0x5FF0) == instruction) { // SE Vx, Vy
      if (device.registers().v[register_x_index] == device.registers().v[register_y_index]) {
        program.GetNextInstruction();
      }
    } else if ((instruction & 0x6FFF) == instruction) { // LD Vx, byte
      device.registers().v[register_x_index] = instruction & 0x00FF;
    } else if ((instruction & 0x7FFF) == instruction) { // ADD Vx, byte
      device.registers().v[register_x_index] += instruction & 0x00FF;
    } else if ((instruction & 0x8FF0) == instruction) { // LD Vx, Vy
      device.registers().v[register_x_index] = device.registers().v[register_y_index];
    } else if ((instruction & 0x8FF1) == instruction) { // OR Vx, Vy
      device.registers().v[register_x_index] |= device.registers().v[register_y_index];
    } else if ((instruction & 0x8FF2) == instruction) { // AND Vx, Vy
      device.registers().v[register_x_index] &= device.registers().v[register_y_index];
    } else if ((instruction & 0x8FF3) == instruction) { // XOR Vx, Vy
      device.registers().v[register_x_index] ^= device.registers().v[register_y_index];
    } else if ((instruction & 0x8FF4) == instruction) { // ADD Vx, Vy
      if (device.registers().v[register_x_index] > 0xFFFF - device.registers().v[register_y_index]) {
        device.registers().v[0xF] = 1;
      } else {
        device.registers().v[0xF] = 0;
      }
      device.registers().v[register_x_index] += device.registers().v[register_y_index];
    } else if ((instruction & 0x8FF5) == instruction) { // SUB Vx, Vy
      if (device.registers().v[register_x_index] > device.registers().v[register_y_index]) {
        device.registers().v[0xF] = 1;
      } else {
        device.registers().v[0xF] = 0;
      }
      device.registers().v[register_x_index] -= device.registers().v[register_y_index];
    } else if ((instruction & 0x8FF6) == instruction) { // SHR Vx, Vy
      int register_y_val = device.registers().v[register_y_index];
      if ((register_y_val & 0x0001) == 0x0001) {
        device.registers().v[0xF] = 1;
      } else {
        device.registers().v[0xF] = 0;
      }
      device.registers().v[register_x_index] = register_y_val >> 1;
    } else if ((instruction & 0x8FF7) == instruction) { // SUBN Vx, Vy
      if (device.registers().v[register_y_index] > device.registers().v[register_x_index]) {
        device.registers().v[0xF] = 1;
      } else {
        device.registers().v[0xF] = 0;
      }
      device.registers().v[register_y_index] -= device.registers().v[register_x_index];
    } else if ((instruction & 0x8FFE) == instruction) { // SHL Vx, Vy
      int register_y_val = device.registers().v[register_y_index];
      if ((register_y_val & 0x8000) == 0x8000) {
        device.registers().v[0xF] = 1;
      } else {
        device.registers().v[0xF] = 0;
      }
      device.registers().v[register_x_index] = register_y_val << 1;
    } else if ((instruction & 0x9FF0) == instruction) { // SNE Vx, Vy
      if (device.registers().v[register_x_index] != device.registers().v[register_y_index]) {
        program.GetNextInstruction();
      }
    } else if ((instruction & 0xAFFF) == instruction) { // LD I, addr
      device.registers().i = instruction & 0x0FFF;
    } else if ((instruction & 0xBFFF) == instruction) { // JP V0, addr
      program.Jump(instruction & 0x0FFF + device.registers().v[0]);
    } else if ((instruction & 0xCFFF) == instruction) { // RND Vx, byte
      // we don't really care about RNG quality so std::rand() suffices
      device.registers().v[register_x_index] = std::rand();
    } else if ((instruction & 0xDFFF) == instruction) { // DRW Vx, Vy, nibble
      device.registers().v[0xF] =
          device.RenderSprite(&device.memory().data[device.registers().i],
                              instruction & 0x000F,
                              device.registers().v[register_x_index],
                              device.registers().v[register_y_index]);
    } else if ((instruction & 0xEF9E) == instruction) { // SKP Vx
      //
    } else if ((instruction & 0xEFA1) == instruction) { // SKNP Vx
      //
    } else if ((instruction & 0xFF07) == instruction) { // LD Vx, DT
      //
    } else if ((instruction & 0xFF0A) == instruction) { // LD Vx, Key
      //
    } else if ((instruction & 0xFF15) == instruction) { // LD DT, Vx
      //
    } else if ((instruction & 0xFF18) == instruction) { // LD ST, Vx
      //
    } else if ((instruction & 0xFF1E) == instruction) { // ADD I, Vx
      //
    } else if ((instruction & 0xFF29) == instruction) { // LD F, Vx
      //
    } else if ((instruction & 0xFF33) == instruction) { // LD B, Vx
      //
    } else if ((instruction & 0xFF55) == instruction) { // LD [I], Vx
      //
    } else if ((instruction & 0xFF65) == instruction) { // LD Vx, [I]
      //
    } else {
      throw std::runtime_error("Unknown instruction!");
    }

    instruction = program.GetNextInstruction();
  }
}

CHIP8::ExecutionThread::ExecutionThread(CHIP8::Device &device, CHIP8::Program &program, bool multi_threaded) {
  std::thread t{ExecuteProgram, std::ref(device), std::ref(program)};
  if (multi_threaded) t.detach();
  else t.join();
}
