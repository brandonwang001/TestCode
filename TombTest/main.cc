#include "mem_file.h"

#include <iostream>
#include <vector>
#include <memory>
#include <assert.h>

std::size_t buf_record_threshold = 5;
std::size_t recored_threshold = 10;
std::size_t file_num = 0;
uint64_t seq_id = 0;
RecordBuf record_buf;


class WalWriter {
 public:
  int Init() {
    mem_log_files_ = std::make_shared<MemFiles<MemLogFile>>();
    mem_index_files_ = std::make_shared<MemFiles<MemIndexFile>>();
    active_tomb_file_ = std::make_shared<MemFiles<MemTombFile>>();
  }
  
  int AppendLog(const Record& record) {
    // create file
    if (!active_file_) {
      active_file_ = std::make_shared<MemLogFile>();
      active_file_->file_num_ = file_num;
      active_index_file_ = std::make_shared<MemIndexFile>();
      active_index_file_->file_num_ = file_num; 
      file_num ++;
    } else {
      if (active_file_->record_blocks_.size() > recored_threshold) {
        mem_log_files_->AddMemFile(active_file_->file_num_, active_file_);
        mem_index_files_->AddMemFile(active_index_file_->file_num_, active_index_file_);
        active_file_ = std::make_shared<MemLogFile>();
        active_file_->file_num_ = file_num;
        active_index_file_ = std::make_shared<MemIndexFile>();
        active_index_file_->file_num_ = file_num;
        file_num++;
      }
    }

    // append buf
    int ret = record_buf.Append(record);
    if (ret != 0) {
      std::cout << "record_buf.Append" << std::endl;
      return -1;
    }

    if (record_buf.record_buf_.size() > buf_record_threshold) {
      // over the buf threshold
      RecordBlock record_block;
      int ret = record_buf.TakeAllRecord(&record_block);
      if (ret != 0) {
        std::cout << "record_buf.TakeAllRecord" << std::endl;
        return ret;
      }
      // write block into file
      uint64_t offset = 0;
      ret = active_file_->AppendRecordBlock(record_block, &offset);
      if (ret != 0) {
        std::cout << "active_file_->AppendRecordBlock" << std::endl;
        return ret;
      }

      std::cout << record_block.Dump() << std::endl;
      std::cout << "offset : " << offset << std::endl;

      // write index into index file
      IndexBlock index_block;
      {
        index_block.tablet_id_ = record_block.tablet_id_;
        index_block.seq_id_start_ = record_block.seq_id_start_;
        index_block.seq_id_end_ = record_block.seq_id_end_;
        index_block.log_id_start_ = record_block.log_id_start_;
        index_block.log_id_end_ = record_block.log_id_end_;
      }
      active_index_file_->AppendIndexBlock(index_block);
    }
    
    return 0;
  }

  int Truncate(uint64_t tablet_id, uint64_t log_id) {
    uint64_t 
    return 0;
  }

  std::string Dump() {
    std::stringstream oss;
    std::shared_ptr<MemLogFile> mem_log_file;
    std::cout << "file_num : " << file_num << std::endl;
    for (uint64_t file_num_temp = 0; file_num_temp < file_num-1; file_num_temp++) {
      std::cout << "dump file_num_temp : " << file_num_temp << std::endl;
      mem_log_files_->GetMemFile(file_num_temp, &mem_log_file);
      oss << mem_log_file->Dump() << std::endl;
    }

    std::shared_ptr<MemIndexFile> mem_index_file;
    for (uint64_t file_num_temp = 0; file_num_temp < file_num-1; file_num_temp++) {
      mem_index_files_->GetMemFile(file_num_temp, &mem_index_file);
      oss << mem_index_file->Dump() << std::endl;
    }
    return oss.str(); 
  }

  std::shared_ptr<MemLogFile> active_file_;
  std::shared_ptr<MemIndexFile> active_index_file_;
  std::shared_ptr<MemTombFile> active_tomb_file_;
  std::shared_ptr<MemFiles<MemLogFile>> mem_log_files_;
  std::shared_ptr<MemFiles<MemIndexFile>> mem_index_files_;
};

int WalWriterCaller() {
  WalWriter wal_writer;
  wal_writer.Init();
  uint64_t tablet_id = 1;
  for (uint64_t log_id = 0; log_id < 1000; log_id++) {
    Record record;
    {
      record.seq_id_ = seq_id;
      record.tablet_id_ = tablet_id;
      record.log_id_ = log_id;
      record.log_ = "hello world";
      std::cout << "tablet_id " << tablet_id << " "
          << "log_id : " << log_id << " "
          << "seq_id : " << seq_id << " " << std::endl;
      seq_id++;
    }
    int ret = wal_writer.AppendLog(record);
    if (ret != 0) {
      std::cout << "wal_writer.AppendLog" << std::endl;
      return ret;
    }
  }
  std::cout << "Ready to Dump()" << std::endl;
  std::cout << wal_writer.Dump() << std::endl;
  return 0; 
}


int Test() {
  assert((WalWriterCaller() ==  0));
}

int main() {
  Test();
  return 0;
}
