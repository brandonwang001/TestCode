#include <iostream>
#include <memory>
#include <vector>
#include <assert.h>

class A {
 public:
  A() {}
  ~A() {
    std::cout << "destructor A" << std::endl;
  }
};

class B {
 public:
  B() {}
  ~B() {}

  void AddEntry() {
    auto a = std::make_shared<A>();
    assert(a);
    vec_.push_back(a);  
  }

  void Print() {
    std::cout << vec_.size() << std::endl;
  }

  void Clear() {
    for (auto& ele : vec_) {
      ele = nullptr;
    }
    vec_.clear();
  }
 private:
  std::vector<std::shared_ptr<A>> vec_;
};


int main() {
  B b;

  for (int i = 0; i < 10; i++) {
    b.AddEntry();
  }

  b.Print();

  b.Clear();

  b.Print();
  return 0;
}
