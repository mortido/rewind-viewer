#pragma once

#include <atomic>
#include <mutex>

namespace rewind_viewer {

class Spinlock {
 public:
  void lock() {
    while (lock_.test_and_set(std::memory_order_acquire)) {
    }
  }

  void unlock() {
    lock_.clear(std::memory_order_release);
  }

 private:
  std::atomic_flag lock_;
};

}  // namespace rewind
