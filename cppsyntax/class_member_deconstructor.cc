#include <iostream>

class A {
 public:
  A() {}
  ~A() {
    std::cout << __func__ << __LINE__
      << " deconstructor working ..." << std::endl;
  }
};

class B {
 public:
  B() {}
  ~B() {
    std::cout << __func__ << __LINE__
      << " deconstructor working ..." << std::endl;
  }
 private:
  A a;
};

int main() {
  B b;
  return 0;
}
