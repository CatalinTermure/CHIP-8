#include "timer.h"

#include <thread>
#include <type_traits>
#include <chrono>

static void ExecuteTimer(CHIP8::Timer *timer, double frequency, std::atomic<bool *> &is_timer_alive) {
  auto start_time = std::chrono::high_resolution_clock::now();

  for (int i = 1; *is_timer_alive; i++) {
    if (i > 1'000'000'000) { // so we don't get overflows
      start_time = std::chrono::high_resolution_clock::now();
      i = 1;
    }

    auto stop_time = start_time + std::chrono::nanoseconds((long long) ((i * 1'000'000'000ll) / frequency));
    std::this_thread::sleep_until(stop_time);

    if (!(*is_timer_alive)) return;

    int value = timer->value();
    if (value > 0) timer->SetValue(value - 1);
  }

  delete is_timer_alive;
}

CHIP8::Timer::Timer(double frequency) {
  is_running_ = new bool;
  *is_running_ = true;
  std::thread t{ExecuteTimer, this, frequency, std::ref(is_running_)};
  t.detach();
}

CHIP8::Timer::~Timer() {
  *is_running_ = false;
}
