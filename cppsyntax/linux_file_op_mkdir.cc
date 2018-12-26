#include <unistd.h>
#include <sys/stat.h>

#include <iostream>
#include <vector>
#include <iterator>
#include <regex>

std::vector<std::string> Split(
  const std::string& in,
  const std::string& delim) {
  std::regex re{ delim };
  return std::vector<std::string> {
    std::sregex_token_iterator(in.begin(), in.end(), re, -1),
    std::sregex_token_iterator()};
}

int CreateDir(const std::string& path) {
  auto dirs = Split(path, "/");
  std::string cur_path;
  for (auto& dir : dirs) {
    if (dir.empty()) {
      cur_path += "/";
    } else {
      cur_path += (dir + "/");
    }
    if (0 != access(cur_path.c_str(), 0)) {
      if (0 != mkdir(cur_path.c_str(), 0755)) {
        return -1;
      }
    }
  }
  return 0;
}

int RemoveDir(const std::string& path) {
  return remove(path.c_str());
}

int main() {
  std::string path{"./tmp///test/./test1/..///test2"};
  std::cout << CreateDir(path) << std::endl; 
  std::string path1{"/tmp///test0/./test1/..///test2"};
  std::cout << CreateDir(path1) << std::endl; 
  std::string path2{"./test"};
  std::cout << CreateDir(path2) << std::endl;

  std::cout << RemoveDir("./tmp/test/test1") << std::endl;
}
