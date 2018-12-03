#include "thread.h"

namespace brandon {

int Thread::Start() {
  int ret = pthread_create(&thread_id_, NULL,
    RunInThread, this);
  if (ret != 0) {
    is_running_ = false;
    return ret;
  }
  is_running_ = true;
  return 0;
}

int Thread::Join() {
  int ret = pthread_join(thread_id_, NULL);
  if (ret != 0) {
    is_running_ = true;
    return ret;
  }
  is_running_ = false;
  return 0;
}

void *Thread::RunInThread(void* args) {
  Thread* thread_ptr = static_cast<Thread*>(args);
  thread_ptr->call_back_();
  return NULL;
}

}  // namespace brandon
