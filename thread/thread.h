#pragma once

#include <pthread.h>

#include <functional>
#include <utility>

namespace brandon {

typedef std::function<void ()> ThreadCallBack;

class Thread {
 public:
  explicit Thread(ThreadCallBack call_back)
  : call_back_(std::move(call_back)),
    thread_id_(0),
    is_running_(false) {
  }

  ~Thread() {
    if (is_running_) {
      pthread_detach(thread_id_);
    }
  }

  int Start();

  int Join();

  static void *RunInThread(void* args);

  Thread(const Thread&) = delete;
  Thread& operator = (const Thread&) = delete;

 private:
  ThreadCallBack call_back_;
  pthread_t thread_id_;
  bool is_running_;
};

}  // namespace brandon
