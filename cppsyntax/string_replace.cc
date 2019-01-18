#include <string>
#include <iostream>

int main() {
  std::string buf;
  buf.replace(0,100,"hello");
  buf.replace(10,15, "hello");
  std::cout << buf << std::endl;
  std::cout << buf.size() << std::endl;
  return 0;
}
