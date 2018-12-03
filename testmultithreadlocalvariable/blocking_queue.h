#pragma once

#include <assert.h>

#include <condition_variable>
#include <deque>
#include <mutex>
#include <utility>

template <typename T>
class BlockingQueue {
 public:
  using MutexLockGuard = std::lock_guard<std::mutex>;

  BlockingQueue()
  : mutex_(), cond_(), queue_() {
  }

  BlockingQueue(const BlockingQueue&) = delete;
  BlockingQueue& operator = (const BlockingQueue&) = delete;

  void Put(const T& x) {
    MutexLockGuard lock(mutex_);
    queue_.push_back(x);
    cond_.notify_one();
  }

  void Put(T&& x) {
    MutexLockGuard lock(mutex_);
    queue_.push_back(std::move(x));
    cond_.notify_one();
  }

  T Take() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.empty()) {
      cond_.wait(lock);
    }
    assert(!queue_.empty());
    T front(std::move(queue_.front()));
    queue_.pop_front();
    return std::move(front);
  }

  size_t Size() const {
    MutexLockGuard lock(mutex_);
    return queue_.size();
  }

 private:
  mutable std::mutex mutex_;
  std::condition_variable cond_;
  std::deque<T> queue_;
};
