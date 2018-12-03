#include <iostream>
#include <stddef.h>
#include <stdint.h>

int main() {
  /*
  uint64_t a = 100;
  char* s = reinterpret_cast<char*>(a); 

  std::string buf(5,'r');
  std::cout << buf << std::endl;

  uint64_t b = 0;
  b = reinterpret_cast<uint64_t>(buf.data()) 
  */


  uint8_t int8 = 10;

  std::string buf;
  char* p = reinterpret_cast<char*>(&int8);
  
  std::cout << std::hex << static_cast<uint16_t>(*p) << std::endl;
  /*
  buf.append(reinterpret_cast<char*>(int8), sizeof(uint8_t));
  std::cout << std::hex << buf << std::endl; 
  */
  return 0;
}
