#include <iostream>
#include <stdint.h>
#include <sstream>
#include <iomanip>
using namespace std;
  template<typename T>
  static void BytesToUint(const string & bytes,
                          T & uint) {
    uint = 0;
    for(int i=0;i<sizeof(T);i++) {
      uint |= bytes.data()[i] << (8 * i);
    }
  }

  template<typename T>
  static void UintToBytes(const T uint,
                          string & bytes) {
    //cout << (char*)&uint << endl;
    cout << sizeof(T) << endl;
    bytes.append((char*)&uint, sizeof(T));
  }


class A {

  public:
    size_t BLOCK_LEN_START  {0};
    size_t BLOCK_LEN_LEN    {sizeof(uint64_t)};
    size_t BLOCK_CRC_START  {BLOCK_LEN_START + BLOCK_LEN_LEN};
};


char ConvertCh(int & ch) {
  cout << ch << endl;
  if(ch >= 0 && ch <= 9 ) {
    return ('0' + ch); 
  }
  if(ch >= 10 && ch <= 15) {
    return ('A'+ (ch-10));
  }
  return 'X';
}

template<typename T>
std::string ConvertHexStr(const T& num) {
  std::stringstream oss;
  for(size_t i=0;i<sizeof(T); i++) {  
    int x = 0;
    x = ((*(((char*)(&num)) + i) & 0x00f0) >> 4);
    oss << ConvertCh(x);
    int y = 0;
    y = (*(((char*)(&num)) + i) & 0x000f);
    oss << ConvertCh(y);
  }
  return oss.str();
}

std::string ConvertHexStr(std::string & buf) {
  std::stringstream oss;
  for(size_t =0; i<buf.size();i++) {
    int x = 0;
    x = (buf[i] & 0x00f0) >> 4);
    oss << ConvertCh(x);
    int y = 0;
    y = (buf[i] & 0x000f);
    oss << ConvertCh(y);
  }
  return oss.str();
}

int main() {
  uint16_t num =2556;
  std::cout << ConverHexStr<uint16_t>(num) << std::endl;
    

  return 0;
}
