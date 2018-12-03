#include <iostream>
#include <utility> 

int main() {
  
  std::pair <int,int> foo;
  
  int a = 10;
  int b = 5;

  foo = std::make_pair(a, b);

  std::cout << "foo: " << foo.first << ", " << foo.second << '\n';

  return 0;
}

