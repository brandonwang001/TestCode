#include <memory>
#include <iostream>

class A {
 public:
  A() {}
  ~A() {
    std::cout << "A is destructor ..." << std::endl;
  }
};

int main() {
  std::shared_ptr<A> a = std::make_shared<A>();
  std::cout << "before a = nullptr" << std::endl;
  a = nullptr;
  std::cout << "after a = nullptr" << std::endl;
  if (a) {
    std::cout << "a is normal" << std::endl;
  } else {
    std::cout << "a is no pointer" << std::endl;
  }
  a = std::make_shared<A>(); 
  return 0;
}
