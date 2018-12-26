#include <iostream>

void FillBuf(char* buf, int* len) {
  *len = 100;
  int i = 0;
  for (i = 0; i < 100; i++ ) {
    buf[i] = 'c';
  }
  buf[i] = '\0';
}

int main() {
  std::string str;
  int len = 0;
  FillBuf(const_cast<char*>(str.data()), &len);
  std::cout << "len : " << len << std::endl;
  std::cout << "str : " << str.data() << std::endl;
  std::cout << "str.size : " << str.size() << std::endl;
}
