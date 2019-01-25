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
}

void PrintMemIndex(
  const WalMemIndex& mem_index) {
  std::string stream_uuid = mem_index.stream_uuid;
  uint64_t seq_id_start = mem_index.seq_id_start;
  uint64_t seq_id_end = mem_index.seq_id_end;
  uint64_t record_id_start = mem_index.record_id_start;
  uint64_t record_id_end = mem_index.record_id_end;
  uint64_t offset = mem_index.offset;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return 0;
  }
  auto dcase = std::strtoul(argv[1], NULL, 10);
  pid_t pid = getpid();
  auto mem_index_ptr = std::make_shared<WalMemIndex>();
  std::cout << sizeof(mem_index_ptr) << std::endl;
  mem_index_ptr->stream_uuid = "12345678901234567890123456789012";
  mem_index_ptr->seq_id_start = 0;
  mem_index_ptr->seq_id_end = 65535;
  mem_index_ptr->record_id_start = 0;
  mem_index_ptr->record_id_end = 65535;
  mem_index_ptr->offset = 65535;
  std::cout << sizeof(mem_index_ptr) << std::endl;
  std::cout << sizeof(*mem_index_ptr) << std::endl;
  typedef std::vector<std::shared_ptr<WalMemIndex>> VecIndexPtr;
  typedef std::vector<WalMemIndex> VecIndex;
  
  auto vec_index_ptr = std::make_shared<VecIndexPtr>();
  auto vec_index = std::make_shared<VecIndex>();

  auto mem_size1 = get_proc_mem(pid);
  if (dcase == 1) {  
    for (int i = 0; i < 1000000; i++) {
      auto mem_index_ptr = std::make_shared<WalMemIndex>();
      mem_index_ptr->stream_uuid = "12345678901234567890123456789012";
      mem_index_ptr->seq_id_start = 0;
      mem_index_ptr->seq_id_end = 65535;
      mem_index_ptr->record_id_start = 0;
      mem_index_ptr->record_id_end = 65535;
      mem_index_ptr->offset = 65535;
      vec_index_ptr->push_back(mem_index_ptr);  
    }
  }
  if (dcase == 2){
    for (int i = 0; i < 1000000; i++) {
      WalMemIndex mem_index;
      mem_index.stream_uuid = "12345678901234567890123456789012";
      mem_index.seq_id_start = 0;
      mem_index.seq_id_end = 65535;
      mem_index.record_id_start = 0;
      mem_index.record_id_end = 65535;
      mem_index.offset = 65535;
      vec_index->push_back(*mem_index_ptr); 
    }
  }
  auto mem_size2 = get_proc_mem(pid);
  std::cout << mem_size2 - mem_size1 << std::endl;
  return 0;
}
