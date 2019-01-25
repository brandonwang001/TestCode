#include <memory>
#include <iostream>
#include <vector>
#include <cstdio>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

#define VMRSS_LINE 17
#define VMSIZE_LINE 13
#define PROCESS_ITEM 14

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
	int vmrss;
	for (int i=0; i<VMRSS_LINE-1;i++){
		fgets(line_buff,sizeof(line_buff),fd);
	}
	
	fgets(line_buff,sizeof(line_buff),fd);
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
  if (dcase == 1) {  
    gettimeofday(&start_time, NULL);
    for (uint32_t i = 0; i < 1000000; i++) {
      auto mem_index_ptr = std::make_shared<WalMemIndex>();
      mem_index_ptr->stream_uuid = stream_uuid;
      mem_index_ptr->seq_id_start = 0;
      mem_index_ptr->seq_id_end = 65535;
      mem_index_ptr->record_id_start = 0;
      mem_index_ptr->record_id_end = 65535;
      mem_index_ptr->offset = 65535;
      vec_index_ptr->push_back(mem_index_ptr);  
    }
    uint64_t mem_used = 0;
    for (uint32_t i = 0; i < 1000000; i++) {
      mem_used += sizeof((*vec_index_ptr)[i]);
      mem_used += (*vec_index_ptr)[i]->stream_uuid.size();
      PrintMemIndex((*vec_index_ptr)[i]);
    }
    std::cout << "mem_used : " << mem_used << std::endl;
    gettimeofday(&end_time, NULL);
    auto runTime = (end_time.tv_sec - start_time.tv_sec ) + (double)(end_time.tv_usec - start_time.tv_usec)/1000000;
    std::cout << "run_time : " << runTime << std::endl;
  }
  if (dcase == 2){
    gettimeofday(&start_time, NULL);
    for (uint32_t i = 0; i < 1000000; i++) {
      WalMemIndex mem_index;
      mem_index.stream_uuid = stream_uuid;
      mem_index.seq_id_start = 0;
      mem_index.seq_id_end = 65535;
      mem_index.record_id_start = 0;
      mem_index.record_id_end = 65535;
      mem_index.offset = 65535;
      vec_index->push_back(mem_index); 
    }
    uint64_t mem_used = 0;
    for (uint32_t i = 0; i < 1000000; i++) {
      mem_used += sizeof((*vec_index)[i]);
      mem_used += (*vec_index)[i].stream_uuid.size();
      PrintMemIndex((*vec_index)[i]);
    }
    std::cout << "mem_used : " << mem_used << std::endl;
    gettimeofday(&end_time, NULL);
    auto runTime = (end_time.tv_sec - start_time.tv_sec ) + (double)(end_time.tv_usec - start_time.tv_usec)/1000000;
    std::cout << "runtime" << runTime << std::endl;
  }
  
  return 0;
}
