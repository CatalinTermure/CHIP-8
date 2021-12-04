#ifndef CHIP8_SRC_INSTRUCTIONS_EXECUTION_THREAD_H_
#define CHIP8_SRC_INSTRUCTIONS_EXECUTION_THREAD_H_

#include "program.h"
#include "../hardware/device.h"

namespace CHIP8 {
class ExecutionThread {
 public:
  ExecutionThread(Device &device, Program &program, bool multi_threaded = true);
};
}
#endif //CHIP8_SRC_INSTRUCTIONS_EXECUTION_THREAD_H_
