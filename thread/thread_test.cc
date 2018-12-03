#include "thread.h"

#include <unistd.h>
#include <queue>
#include <iostream>

std::queue<int> g_queue;

class Producer {
 public:
  Producer() {}
  ~Producer() {}

 public:
  void Produce() {
    for (int i = 0; i < 10; i++) {
      g_queue.push(i);
      std::cout << "Produce : " << i << std::endl;
    }
  }
};

class Consumer {
 public:
  Consumer() {}
  ~Consumer() {}

 public:
  void Consume() {
    while (true) {
      if (!g_queue.empty()) {
        int x = g_queue.front();
        std::cout << "Consume : " << x << std::endl;
        g_queue.pop();
      }
    }
  }
};

int main() {
  Producer producer;
  brandon::Thread producer_thread(
    std::bind(&Producer::Produce, &producer));

  Consumer consumer;
  brandon::Thread consumer_thread(
    std::bind(&Consumer::Consume, &consumer));

  producer_thread.Start();
  consumer_thread.Start();

  while (true) {
    usleep(100*1000);
  }

  producer_thread.Join();
  consumer_thread.Join();

  return 0;
}
