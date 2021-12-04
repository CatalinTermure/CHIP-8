#ifndef CHIP8_SRC_HARDWARE_TIMER_H_
#define CHIP8_SRC_HARDWARE_TIMER_H_

#include <atomic>
#include <thread>

namespace CHIP8 {
class Timer {
 public:
  explicit Timer(double frequency);
  ~Timer();

  void SetValue(int cycles) { cycles_ = cycles; };
  [[nodiscard]] int value() const { return cycles_; }

  Timer(Timer &other) = delete;
  Timer &operator=(Timer &other) = delete;
 private:
  std::atomic<bool *> is_running_;
  std::atomic<int> cycles_;
};
}

#endif //CHIP8_SRC_HARDWARE_TIMER_H_
