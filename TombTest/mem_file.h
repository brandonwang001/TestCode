#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <sstream>
#include <algorithm>

class Record {
 public:
  Record() {}
  
  ~Record() {} 

  std::string Dump() {
    std::cout << "Ready to dump Record" << std::endl;
    std::stringstream oss;
    oss << "Record {" << std::endl;
    oss << "seq_id : " << seq_id_ << std::endl;
    oss << "tablet_id_ : " << tablet_id_ << std::endl;
    oss << "log_id_ : " << log_id_ << std::endl;
    oss << "log : " << log_ << std::endl;
    oss << "}" << std::endl;
    return oss.str();
  }

  uint64_t seq_id_;
  uint64_t tablet_id_;
  uint64_t log_id_;
  std::string log_; 
};

class RecordBlock {
 public:
  RecordBlock() {}

  ~RecordBlock() {}

  int AppendRecord(const Record& record) {
    records_.push_back(record);
    return 0;
  }

  int GetRecord(uint64_t log_id, Record* record) {
    for (auto ele : records_) {
      if (ele.log_id_  == log_id) {
        *record = ele;
        return 0;
      }
    }
    return -1;
  }
  
  std::string Dump() {
    std::cout << "Ready to dump RecordBlock" << std::endl;
    std::stringstream oss;
    oss << "RecordBlock {" << std::endl;
    oss << "tablet_id_ : " << tablet_id_ << std::endl;
    oss << "log_id_start_ : " << log_id_start_ << std::endl;
    oss << "log_id_end_ : " << log_id_end_ << std::endl;
    oss << std::endl;
    for (auto& record : records_) {
      oss << record.Dump();
    }
    oss << std::endl;
    oss << "}" << std::endl;
    return oss.str();
  }


  uint64_t tablet_id_;
  uint64_t log_id_start_;
  uint64_t log_id_end_;
  uint64_t seq_id_start_;
  uint64_t seq_id_end_;
  std::vector<Record> records_;
};

class RecordBuf {
 public:
  RecordBuf() {
    write_flag_ = true;  
  }
  int Append(const Record& record) {
    if (write_flag_) {
      record_buf_.push_back(record);
    } else {
      return -1;
    }
    return 0;
  }

  int TakeAllRecord(RecordBlock* record_block) {
    if (record_buf_.size() > 0) {
      write_flag_ = false;
      uint64_t tablet_id = record_buf_[0].tablet_id_;
      std::vector<uint64_t> log_ids;
      std::vector<uint64_t> seq_ids;

      for (auto& ele : record_buf_) {
        log_ids.push_back(ele.log_id_);
        seq_ids.push_back(ele.seq_id_);
        record_block->AppendRecord(ele);
      }
      {
        record_block->tablet_id_ = tablet_id;
        record_block->log_id_start_ = *std::min_element(log_ids.begin(), log_ids.end());
        record_block->log_id_end_ = *std::max_element(log_ids.begin(), log_ids.end());
        record_block->seq_id_start_ = *std::min_element(seq_ids.begin(), seq_ids.end());
        record_block->seq_id_end_ = *std::max_element(seq_ids.begin(), seq_ids.end());
      }
      record_buf_.clear();
      write_flag_ = true;
    } else {
      return 0;
    }
    return 0;
  }

  std::vector<Record> record_buf_;
  bool  write_flag_;
};

class MemLogFile {
 public:
  MemLogFile() {
    record_nums_ = 0;
  }

  ~MemLogFile() {}

  int AppendRecordBlock(const RecordBlock& record_block, uint64_t* offset) {
    *offset = record_nums_;
    record_blocks_.push_back(record_block);
    record_nums_++;
    return 0;
  }

  int GetRecordBlock(uint64_t offset, RecordBlock* record_block) {
    if (offset > record_blocks_.size()) {
      return -1;
    }
    *record_block = record_blocks_[offset];
    return 0;
  }

  std::string Dump() {
    std::cout << "Ready to dump MemLogFile file_num_ : " << file_num_ << std::endl;
    std::stringstream oss;
    oss << "MemLogFile {" << std::endl;
    oss << "file_num_ : " << file_num_ << std::endl;
    oss << std::endl;
    for (auto& record_block : record_blocks_) {
      oss << record_block.Dump(); 
    }
    oss << std::endl;  
    oss << "}" << std::endl;   
    return oss.str();
  }
  
  uint64_t record_nums_;
  uint64_t file_num_;
  std::vector<RecordBlock> record_blocks_;
};


class IndexBlock {
 public:
  uint64_t tablet_id_;
  uint64_t seq_id_start_;
  uint64_t seq_id_end_;
  uint64_t log_id_start_;
  uint64_t log_id_end_;
  uint64_t offset_;

  std::string Dump() {
    std::stringstream oss;
    oss << "IndexBlock {" << std::endl;
    oss << "tablet_id_ : " << tablet_id_ << std::endl;
    oss << "seq_id_start_ : " << seq_id_start_ << std::endl;
    oss << "seq_id_end_ : " << seq_id_end_ << std::endl;
    oss << "log_id_start_ : " << log_id_start_ << std::endl;
    oss << "log_id_end_ : " << log_id_end_ << std::endl;
    oss << "}" << std::endl;
    return oss.str();
  }
};

class MemIndexFile {
 public:
  
  int AppendIndexBlock(const IndexBlock& index_block) {
    index_blocks_.push_back(index_block);
    return 0;
  }

  std::string Dump() {
    std::stringstream oss;
    oss << "MemIndexFile {" << std::endl;
    for (auto& index_block : index_blocks_) {
      oss << index_block.Dump() << std::endl;
    }
    oss << "}" << std::endl;
    return oss.str(); 
  }
  
  uint64_t file_num_;
  std::vector<IndexBlock> index_blocks_;
};


class TombBlock {
 public:
  uint64_t tablet_id;
  uint64_t seq_id_start;
  uint64_t seq_id_end;

  std::string Dump() {
    std::stringstream oss;
    oss << "TombBlock {" << std::endl;
    oss << "tablet_id : " << tablet_id << std::endl;
    oss << "seq_id_start : " << seq_id_start << std::endl;
    oss << "seq_id_end : " << seq_id_end << std::endl;
    oss << "}" << std::endl;
  }
};

class MemTombFile {
 public:
  int AppendTombBlock(const TombBlock& tomb_block) {
    tomb_blocks_.push_back(tomb_block);
    return 0;
  }
  
  std::vecotr<TombBlock> tomb_blocks_;
};

template<typename T>
class MemFiles {
 public:

  int AddMemFile(uint64_t file_num, std::shared_ptr<T>& mem_file) {
    mem_files_.insert(std::make_pair(file_num, mem_file));
    return 0;
  }

  int GetMemFile(uint64_t file_num, std::shared_ptr<T>* mem_file) {
    std::cout << "GetMemFile file_num : " << file_num << std::endl;
    *mem_file = mem_files_[file_num];
    return 0;
  }

  std::unordered_map<uint64_t, std::shared_ptr<T>> mem_files_;
};
