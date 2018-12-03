#include "blocking_queue.h"
#include <iostream>

#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#include <atomic>
#include <mutex>
#include <vector>

BlockingQueue<int> task_queue_; 
class Log {
 public:
  Log() {}
  ~Log() {}

  void Init(void*(*func)(void*)) {
    int consumer_num = 1;
    std::vector<pthread_t> consumers;
    consumers.resize(consumer_num);
    for (int i = 0; i < consumer_num; i++) {
      int ret = pthread_create(&consumers[i],
        NULL, func, NULL);
    }
  }

  void AppendLog(int log) {
    task_queue_.Put(log);
  }
};

Log log;

void* Consumer(void*) {
  while (true) {
    std::cout << task_queue_.Take() << std::endl;
  }
}

void* Producer(void*) {
  for (int i = 0; i < 10; i++) {
    log.AppendLog(i);
  }
}

int main() {
  log.Init(&Consumer);

  int producer_num = 3;
  std::vector<pthread_t> producers;
  producers.resize(producer_num);
  for (int i = 0; i < producer_num; i++) {
    int ret = pthread_create(&producers[i],
      NULL, &Producer, NULL);
  }

  while (true) {
    usleep(1000*1000);
  }
  for (int i = 0; i < producer_num; i++) {
    pthread_detach(producers[i]);
  }
  return 0;
}
