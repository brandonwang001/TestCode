#include <iostream>
#include <memory>
#include <mutex>

template <typename T>
class ResourceProtect {
 public:
  using MutexLockGuard = std::lock_guard<std::mutex>;
  
  ResourceProtect(const T& x) 
  : mutex_(),
    x_(x) {
  }

  ~ResourceProtect() {}

 public:
  void Set(const T& x) {
    MutexLockGuard lock(mutex_);
    x_ = x;
  }

  T& Get() {
    MutexLockGuard lock(mutex_);
    return x_;
  }

 private:
  std::mutex mutex_;
  T x_;
};


int main() {
  auto y = std::make_shared<int>(5);
  ResourceProtect<std::shared_ptr<int>> rp(y);

  std::cout << *rp.Get() << std::endl;
  return 0;
}
