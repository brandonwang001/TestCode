#include "queue.h"
#include <iostream>

#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#include <atomic>
#include <mutex>
#include <vector>
bytable::Queue<uint64_t> g_queue;
std::atomic<uint64_t> num_range(1);
int producer_num = 3;
uint64_t num_sum = producer_num * (num_range * (num_range + 1) / 2);
int consumer_num = 1;
std::atomic<uint64_t> g_num_sum(0);
using MutexLockGuard = std::lock_guard<std::mutex>;
std::mutex g_mutex;

uint64_t GetCurrentTime() {
  MutexLockGuard lock(g_mutex);
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void* Producer(void*) {
  uint64_t time_start = GetCurrentTime();
  for (uint64_t i = 1; i <= num_range; i++) {
    g_queue.put(i);
  }
  uint64_t time_end = GetCurrentTime();
  return reinterpret_cast<void*>(NULL);
}

void* Consumer(void*) {
  uint64_t handle_num  = 0;
  uint64_t time_start = GetCurrentTime();
  while (true) {
    uint64_t num = 0;
    bool exist = g_queue.take(&num);
    if (exist) {
      g_num_sum.fetch_add(num);
      handle_num++;
    }
  }
  uint64_t time_end = GetCurrentTime();
  return reinterpret_cast<void*>(NULL);
}

void Observer() {
  while (true) {
    if (num_sum == g_num_sum) {
      std::cout << "bingo" << std::endl;
      return;
    }
    usleep(100 * 1000);
  }
}
int main() {
  int* p = new int(5);
  std::cout << p << std::endl;
  std::vector<pthread_t> producers;
  producers.resize(producer_num);
  for (int i = 0; i < producer_num; i++) {
    int ret = pthread_create(&producers[i],
      NULL, &Producer, NULL);
  }
  std::cout << "producer" << std::endl;

  std::vector<pthread_t> consumers;
  consumers.resize(consumer_num);
  for (int i = 0; i < consumer_num; i++) {
    int ret = pthread_create(&consumers[i],
      NULL, &Consumer, NULL);
  }
  std::cout << "consumer" << std::endl;

  Observer();
  std::cout << "observer" << std::endl;

  for (int i = 0; i < producer_num; i++) {
    pthread_detach(producers[i]);
  }

  for (int i = 0; i < consumer_num; i++) {
    pthread_detach(consumers[i]);
  }
  return 0;
}
