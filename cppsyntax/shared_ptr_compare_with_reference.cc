#include <memory>
#include <iostream>
#include <vector>
#include <cstdio>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <iomanip>
#include <sstream>

#define VMRSS_LINE 20
#define VMSIZE_LINE 13
#define PROCESS_ITEM 14

//g++ shared_ptr_compare_with_reference.cc -o main --std=c++11

unsigned int get_proc_mem(unsigned int pid){
	char file_name[64]={0};
	FILE *fd;
	char line_buff[512]={0};
	sprintf(file_name,"/proc/%d/status",pid);
	
	fd =fopen(file_name,"r");
	if(nullptr == fd){
		return 0;
	}
	
	char name[64];
	uint32_t vmrss;
	for (int i=0; i<VMRSS_LINE;i++){
		fgets(line_buff,sizeof(line_buff),fd);
	}
	
	fgets(line_buff,sizeof(line_buff),fd);
  std::cout << line_buff << std::endl;
	sscanf(line_buff,"%s %d",name,&vmrss);
  
  fclose(fd);
	return vmrss;
}

struct WalMemIndex {
 public:
  std::string stream_uuid = "";
  uint64_t seq_id_start = 0;
  uint64_t seq_id_end = 0;
  uint64_t record_id_start = 0;
  uint64_t record_id_end = 0;
  uint64_t offset = 0;
};

void PrintMemIndex(
  const std::shared_ptr<WalMemIndex>& mem_index) {
  std::string stream_uuid = mem_index->stream_uuid;
  uint64_t seq_id_start = mem_index->seq_id_start;
  uint64_t seq_id_end = mem_index->seq_id_end;
  uint64_t record_id_start = mem_index->record_id_start;
  uint64_t record_id_end = mem_index->record_id_end;
  uint64_t offset = mem_index->offset;
  uint64_t sum = seq_id_start + seq_id_end + record_id_start +
      record_id_end + offset;
}

void PrintMemIndex(
  const WalMemIndex& mem_index) {
  std::string stream_uuid = mem_index.stream_uuid;
  uint64_t seq_id_start = mem_index.seq_id_start;
  uint64_t seq_id_end = mem_index.seq_id_end;
  uint64_t record_id_start = mem_index.record_id_start;
  uint64_t record_id_end = mem_index.record_id_end;
  uint64_t offset = mem_index.offset;
  uint64_t sum = seq_id_start + seq_id_end + record_id_start +
      record_id_end + offset;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    return 0;
  }
  auto times = std::strtoul(argv[2], NULL, 10);
  std::string stream_uuid(times, '1');
  auto dcase = std::strtoul(argv[1], NULL, 10);
  pid_t pid = getpid();
  
  typedef std::vector<std::shared_ptr<WalMemIndex>> VecIndexPtr;
  typedef std::vector<WalMemIndex> VecIndex;
  
  auto vec_index_ptr = std::make_shared<VecIndexPtr>();
  auto vec_index = std::make_shared<VecIndex>();
  struct timeval start_time;
  struct timeval end_time;
  auto mem_size1 = get_proc_mem(pid);
  if (dcase == 1) {  
    gettimeofday(&start_time, NULL);
    for (uint32_t i = 0; i < 1000000; i++) {
      auto mem_index_ptr = std::make_shared<WalMemIndex>();
      std::stringstream oss;
      oss << std::setfill('@') << std::setw(times) << i << std::endl;
      mem_index_ptr->stream_uuid = oss.str();
      mem_index_ptr->seq_id_start = 0;
      mem_index_ptr->seq_id_end = 65535;
      mem_index_ptr->record_id_start = 0;
      mem_index_ptr->record_id_end = 65535;
      mem_index_ptr->offset = 65535;
      vec_index_ptr->push_back(mem_index_ptr);  
    }
    for (uint32_t i = 0; i < 1000000; i++) {
      PrintMemIndex((*vec_index_ptr)[i]);
    }
    gettimeofday(&end_time, NULL);
    auto runTime = (end_time.tv_sec - start_time.tv_sec ) + (double)(end_time.tv_usec - start_time.tv_usec)/1000000;
    std::cout << "run_time : " << runTime << std::endl;
  }
  if (dcase == 2){
    gettimeofday(&start_time, NULL);
    for (uint32_t i = 0; i < 1000000; i++) {
      WalMemIndex mem_index;
      std::stringstream oss;
      oss << std::setfill('@') << std::setw(times) << i << std::endl;
      mem_index.stream_uuid = oss.str();
      mem_index.seq_id_start = 0;
      mem_index.seq_id_end = 65535;
      mem_index.record_id_start = 0;
      mem_index.record_id_end = 65535;
      mem_index.offset = 65535;
      vec_index->push_back(mem_index); 
    }
    for (uint32_t i = 0; i < 1000000; i++) {
      PrintMemIndex((*vec_index)[i]);
    }
    gettimeofday(&end_time, NULL);
    auto runTime = (end_time.tv_sec - start_time.tv_sec ) + (double)(end_time.tv_usec - start_time.tv_usec)/1000000;
    std::cout << "runtime : " << runTime << std::endl;
  }
  auto mem_size2 = get_proc_mem(pid);
  std::cout << "VmRss : " << mem_size2 - mem_size1 << std::endl; 
  return 0;
}
