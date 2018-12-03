#pragma once

#include <assert.h>

#include <iostream>
#include <deque>
#include <mutex>
#include <utility>

namespace bytable {

template <typename T>
class Queue {
 public:
  using MutexLockGuard = std::lock_guard<std::mutex>;

  Queue()
  : mutex_(), queue_() {
  }

  Queue(const Queue&) = delete;
  Queue& operator = (const Queue&) = delete;

  void put(const T& x) {
    int y = 0;
    std::cout << &y << std::endl;
    MutexLockGuard lock(mutex_);
    queue_.push_back(x);
  }

  void put(T&& x) {
    MutexLockGuard lock(mutex_);
    queue_.push_back(std::move(x));
  }

  bool take(T* x) {
    MutexLockGuard lock(mutex_);
    if (!queue_.empty()) {
      T front(std::move(queue_.front()));
      queue_.pop_front();
      *x = std::move(front);
      return true;
    } else {
      return false;
    }
  }

  size_t size() const {
    MutexLockGuard lock(mutex_);
    return queue_.size();
  }

 private:
  mutable std::mutex mutex_;
  std::deque<T> queue_;
};

}  // namespace bytable
