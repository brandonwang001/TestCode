#include<iostream>

using namespace std;


class FDGuard {
  public:
  FDGuard() : fd_(-1) {}
  FDGuard(int fd) : fd_(fd) {}
  ~FDGuard(){};

  operator bool() const{
    //string sTemp = "hello world";
    //return sTemp;
    return 54;
  }

  private:
    int fd_;
};



int main()
{
  cout << "hello world" << endl;

  FDGuard fguard(5);

  cout << "fguard : " << fguard << endl;
  return 0;
}
