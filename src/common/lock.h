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

template <typename T, typename M>
class ScopeLockedRefWrapper {
 public:
  ScopeLockedRefWrapper(T& resource, M& mutex) : resource_(resource), lock_(mutex) {}

  T* operator->() {
    return &resource_;
  }

  T& operator*() {
    return resource_;
  }

  // Delete copy and move operations to prevent misuse
  ScopeLockedRefWrapper(const ScopeLockedRefWrapper&) = delete;
  ScopeLockedRefWrapper& operator=(const ScopeLockedRefWrapper&) = delete;
  ScopeLockedRefWrapper(ScopeLockedRefWrapper&&) = delete;
  ScopeLockedRefWrapper& operator=(ScopeLockedRefWrapper&&) = delete;

 private:
  T& resource_;
  std::lock_guard<M> lock_;
};

}  // namespace rewind
