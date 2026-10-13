#pragma once

#include <cstddef>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>

namespace botf::net {

// Thread-safe FIFO for notifications between the network layer and game logic.
template <typename T>
class NotificationQueue {
 public:
  void push(T item) {
    std::lock_guard lock(mutex_);
    queue_.push(std::move(item));
  }

  std::optional<T> try_pop() {
    std::lock_guard lock(mutex_);
    if (queue_.empty()) {
      return std::nullopt;
    }
    T item = std::move(queue_.front());
    queue_.pop();
    return item;
  }

  [[nodiscard]] bool empty() const {
    std::lock_guard lock(mutex_);
    return queue_.empty();
  }

  [[nodiscard]] std::size_t size() const {
    std::lock_guard lock(mutex_);
    return queue_.size();
  }

 private:
  mutable std::mutex mutex_;
  std::queue<T> queue_;
};

}  // namespace botf::net
