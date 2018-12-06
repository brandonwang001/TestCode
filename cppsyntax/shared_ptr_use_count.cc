#include <iostream>
#include <memory>
using namespace std;

class Log {
 public:
  Log(int x) {
    std::cout << " Log constructor is working ..." << std::endl;
    x_ = x;
  }
  ~Log() {
    std::cout << " Log deconstructor is working..." << std::endl;  
  }

 private:
  int x_;
};

class Entry {
 public:
  Entry() {
    std::cout << " Entry constructor is working ..." << std::endl;
    std::cout << "log_.use_count = " << log_.use_count() << std::endl;
  }
  ~Entry() {
    std::cout << " Entry deconstructor is working ..." << std::endl; 
    std::cout << "log_.use_count = " << log_.use_count() << std::endl;
  }
  
  void AddLog(std::shared_ptr<Log> log) {
    std::cout << "log.use_count = " << log.use_count() << std::endl;
    std::cout << "log_.use_count = " << log_.use_count() << std::endl;
    log_ = log;
    std::cout << "log_.use_count = " << log_.use_count() << std::endl;
    std::cout << "log.use_count = " << log.use_count() << std::endl;
  }

 private:
  std::shared_ptr<Log> log_;
};

int main() {
  std::shared_ptr<Log> log_tmp;
  {
    auto log = std::make_shared<Log>(5);
    log_tmp = log;
    {
      std::cout << "log.use_count = " << log.use_count() << std::endl;
      Entry entry;
      entry.AddLog(log);
    }
    std::cout << "log.use_count = " << log.use_count() << std::endl;
  }
  std::cout << "log_tmp.use_count = " << log_tmp.use_count() << std::endl;
  return 0;
}
