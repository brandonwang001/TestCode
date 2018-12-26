#include <atomic>
#include <cstddef>
#include <iostream>

template <typename T>
class WorkStealingQueue {
public:
      WorkStealingQueue()
        : _bottom(1)
        , _capacity(0)
        , _buffer(NULL)
        , _top(1) {
    }

    ~WorkStealingQueue() {
        delete [] _buffer;
        _buffer = NULL;
    }

    int init(unsigned int capacity) {
        if (_capacity != 0) {
            return -1;
        }
        if (capacity == 0) {
            return -1;
        }
        if (capacity & (capacity - 1)) {
            return -1;
        }
        _buffer = new(std::nothrow) T[capacity];
        if (NULL == _buffer) {
            return -1;
        }
        _capacity = capacity;
        return 0;
    }

    // Push an item into the queue.
    // Returns true on pushed.
    // May run in parallel with steal().
    // Never run in parallel with pop() or another push().
    bool push(const T& x) {
        const unsigned int b = _bottom.load();
        const unsigned int t = _top.load();
        if (b >= t + _capacity) { // Full queue.
            return false;
        }
        _buffer[b & (_capacity - 1)] = x;
        _bottom.store(b + 1);
        if ((t % (1000 * 1000)) == 0) {
          std::cout << "_top : " << t << std::endl;
        }
        return true;
    }

    // Pop an item from the queue.
    // Returns true on popped and the item is written to `val'.
    // May run in parallel with steal().
    // Never run in parallel with push() or another pop().
    bool pop(T* val) {
        const unsigned int b = _bottom.load();
        unsigned int t = _top.load();
        if (t >= b) {
            // fast check since we call pop() in each sched.
            // Stale _top which is smaller should not enter this branch.
            return false;
        }
        const unsigned int newb = b - 1;
        _bottom.store(newb);
        t = _top.load();
        if (t > newb) {
            _bottom.store(b);
            return false;
        }
        *val = _buffer[newb & (_capacity - 1)];
        if (t != newb) {
            return true;
        }
        // Single last element, compete with steal()
        const bool popped = _top.compare_exchange_strong(t, t + 1);
        _bottom.store(b);
        return popped;
    }

    // Steal one item from the queue.
    // Returns true on stolen.
    // May run in parallel with push() pop() or another steal().
    bool steal(T* val) {
        unsigned int t = _top.load();
        unsigned int b = _bottom.load();
        if (t >= b) {
            // Permit false negative for performance considerations.
            return false;
        }
        do {
            b = _bottom.load();
            if (t >= b) {
                return false;
            }
            *val = _buffer[t & (_capacity - 1)];
        } while (!_top.compare_exchange_strong(t, t + 1));
        return true;
    }

    unsigned int volatile_size() const {
        const unsigned int b = _bottom.load();
        const unsigned int t = _top.load();
        return (b <= t ? 0 : (b - t));
    }

    unsigned int capacity() const { return _capacity; }

private:
    std::atomic<unsigned int> _bottom;
    unsigned int _capacity;
    T* _buffer;
    std::atomic<unsigned int> _top;
};


int main() {
  WorkStealingQueue<int> work_steal_queue;
  work_steal_queue.init(8);

  while (work_steal_queue.push(5)) {
    int x;
    work_steal_queue.steal(&x);
  }
  return 0;
}
