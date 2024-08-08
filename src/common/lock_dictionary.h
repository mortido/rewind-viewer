#pragma once

#include <functional>
#include <mutex>
#include <utility>
#include <vector>

#include "common/lock.h"

namespace rewind_viewer {

template <typename KeyT, typename ValueT>
class LockDictionary {
 public:
  using Callback = std::function<void(const KeyT&, ValueT&)>;
  using ConstCallback = std::function<void(const KeyT&, const ValueT&)>;

  void iterate(Callback callback) {
    std::lock_guard lock(mutex_);
    for (auto& [key, item] : items_) {
      callback(key, item);
    }
  }

  void iterate(Callback callback) const {
    std::lock_guard lock(mutex_);
    for (const auto& [key, item] : items_) {
      callback(key, item);
    }
  }

  void remove(const KeyT& key) {
    std::lock_guard lock(mutex_);
    items_.erase(
        std::remove_if(items_.begin(), items_.end(),
                       [&key](const std::pair<KeyT, ValueT>& item) { return item.first == key; }),
        items_.end());
  }

  void clear() {
    std::lock_guard lock(mutex_);
    items_.clear();
  }

  void add(const KeyT& key, ValueT item) {
    std::lock_guard lock(mutex_);
    items_.erase(
        std::remove_if(items_.begin(), items_.end(),
                       [&key](const std::pair<KeyT, ValueT>& item) { return item.first == key; }),
        items_.end());
    items_.emplace_back(key, std::move(item));
  }

  bool empty() const { return items_.empty(); }

 private:
  mutable Spinlock mutex_;
  std::vector<std::pair<KeyT, ValueT>> items_;
};

}  // namespace rewind_viewer
