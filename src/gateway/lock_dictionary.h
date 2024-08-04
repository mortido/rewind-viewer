#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <utility>

#include "common/lock.h"

namespace rewind_viewer::gateway {

template <typename KeyT, typename ValueT>
class LockDictionary {
 public:
  using Callback = std::function<void(const KeyT&, ValueT&)>;

  void iterate(Callback callback) {
    std::lock_guard lock(mutex_);
    for (auto& [key, item] : items_) {
      callback(key, item);
    }
  }

  void remove(const KeyT& key) {
    std::lock_guard lock(mutex_);
    items_.erase(key);
  }

  void clear() {
    std::lock_guard lock(mutex_);
    items_.clear();
  }

  void add(const KeyT& key, ValueT item) {
    std::lock_guard lock(mutex_);
    items_.emplace(key, std::move(item));
  }

  bool empty() const { return items_.empty(); }

 private:
  mutable Spinlock mutex_;
  std::map<KeyT, ValueT> items_;
};

}  // namespace rewind_viewer::gateway
