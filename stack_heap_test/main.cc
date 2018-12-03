#include <iostream>
#include <vector>
using namespace std;

class A {

  public:
    void print() {
      cout << "A-a : " << std::hex << &a << endl;
      cout << "A-b : " << std::hex << &b << endl; 
      c = new int(100);   
      cout << "A-c : " << std::hex << c << endl;
      //delete c;
      c[99] = 10;
      d = new int(100);
      cout << "A-d : " << std::hex << d << endl; 
    }
  
  private:
    int a;
    int b;
    int * c;
    int * d;
};


int main() {
  int a = 1;
 
  cout << "a : " << std::hex << &a << endl;    
  
  int *b = new int;
  cout << "b : " << std::hex << b << endl;    
  delete b;
  
  A a_stack;
  a_stack.print();

  A * a_heap = new A();
  a_heap->print();
  delete a_heap;

  return 0;  
}
