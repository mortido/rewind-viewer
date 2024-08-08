#pragma once

#include <functional>
#include <mutex>
#include <utility>
#include <vector>

#include "common/lock.h"

namespace rewind_viewer {

template <typename ValueT>
class LockVector {
 public:
  using Callback = std::function<void(ValueT&)>;
  using ConstCallback = std::function<void(const ValueT&)>;

  void iterate(Callback callback) {
    std::lock_guard lock(mutex_);
    for (auto& [key, item] : items_) {
      callback(key, item);
    }
  }

  void iterate(ConstCallback callback) const {
    std::lock_guard lock(mutex_);
    for (const auto& item : items_) {
      callback(item);
    }
  }

  void transfer_from(LockVector<ValueT>& other) {
    items_.insert(items_.end(), std::make_move_iterator(other.items_.begin()),
                     std::make_move_iterator(other.items_.end()));
    other.items_.clear();
  }

  void clear() {
    std::lock_guard lock(mutex_);
    items_.clear();
  }

  void emplace(ValueT&& item) {
    std::lock_guard lock(mutex_);
    items_.emplace_back(std::move(item));
  }

  bool empty() const { return items_.empty(); }

 private:
  mutable Spinlock mutex_;
  std::vector<ValueT> items_;
};

}  // namespace rewind_viewer
