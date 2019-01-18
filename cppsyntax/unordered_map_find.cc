#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <sys/time.h>

class Index {
 public:
  int AppendIndex(const std::string stream_uuid,
      uint64_t value) {
    std::shared_ptr<std::vector<uint64_t>> index_vector;
    auto ret = FindIndexVector(stream_uuid, &index_vector);
    if (ret != 0) {
      index_vector = std::make_shared<std::vector<uint64_t>>();
      hash_map_.emplace(stream_uuid, index_vector);
    }
    index_vector->push_back(value);
    return 0;
  }

  int FindIndex1(const std::string stream_uuid) {
    uint64_t offset = 0;
    while (true) {
      std::shared_ptr<std::vector<uint64_t>> index_vector;
      auto ret = FindIndexVector(stream_uuid, &index_vector);
      if (ret == 0) {
        uint64_t value = 0;
        ret = ReadIndex(index_vector, offset, &value);
        if (ret == 0) {
          offset++;
        } else {
          break; 
        }
      } else {
        break;
      }
    }
    return 0;
  }

  int FindIndex2(const std::string stream_uuid) {
    uint64_t offset = 0;
    std::shared_ptr<std::vector<uint64_t>> index_vector;
    auto ret = FindIndexVector(stream_uuid, &index_vector);
    if (ret == 0) {
      while(true) {
        uint64_t value = 0;
        ret = ReadIndex(index_vector, offset, &value);
        if (ret == 0) {
          offset++;
        } else {
          break; 
        }
      }
    } else {
      return -1;
    }
    return 0;
  }

  int FindIndexVector(const std::string stream_uuid,
      std::shared_ptr<std::vector<uint64_t>>* index_vector) {
     auto ele = hash_map_.find(stream_uuid);
     if (ele == hash_map_.end()) {
        return -1;
     } else {
       *index_vector = ele->second;
     }
     return 0;
  }

  int ReadIndex(const std::shared_ptr<std::vector<uint64_t>>& index_vector,
      uint64_t offset, uint64_t* value) {
    if (offset < index_vector->size()) {
      *value = (*index_vector)[offset];
      return 0;
    } else {
      return -1;
    }
  }

 private:
  std::unordered_map<std::string, std::shared_ptr<std::vector<uint64_t>>> hash_map_;
};

int main() {

  Index index;
  int stream_num = 1000;
  for (int i = 0; i < stream_num; i++) {
    std::string stream_uuid = "3.14159263.14159263.14159263.1415926" + std::to_string(i);
    uint64_t value = 0;
    index.AppendIndex(stream_uuid, value);
  }

  int index_num = 10000;
  std::string stream_uuid = "3.14159263.14159263.14159263.1415926" + std::to_string(500);
  for (int i = 0; i < index_num; i++) {
    index.AppendIndex(stream_uuid, i); 
  }

  struct timeval start_time;
  struct timeval end_time;
  gettimeofday(&start_time, NULL);
  for (int i = 0; i < 10000; i++) {
    index.FindIndex1(stream_uuid);
  }
  gettimeofday(&end_time, NULL);
  std::cout << end_time.tv_sec - start_time.tv_sec << std::endl;

  gettimeofday(&start_time, NULL);
  for (int i = 0; i < 10000; i++) {
    index.FindIndex2(stream_uuid);
  }
  gettimeofday(&end_time, NULL);
  std::cout << end_time.tv_sec - start_time.tv_sec << std::endl;
  return 0;
}
