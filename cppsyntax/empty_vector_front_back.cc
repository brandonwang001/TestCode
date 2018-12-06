#include <iostream>
#include <vector>

int main() {
  std::vector<int> vec;
  // if vector is empty, the below operation will lead the program to coredump.
  // vec.push_back(1);
  std::cout << vec.front() << std::endl;
  std::cout << vec.back() << std::endl;
  return 0;
}
