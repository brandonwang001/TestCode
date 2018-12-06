#include <iostream>

class A {
 public:
  A(int a)
  : a_(a) {}
  ~A() {}
 
 int GetMember() /* const */ {
  return a_;
 }
 
 private:
  int a_;
};

void Print(const A& a) {
  std::cout << a.GetMember() << std::endl;
}

int main() {
  A a(10);
  Print(a);   
  return 0;
}
