#pragma once
#include <cstring>
#include <memory>
#include <string_view>
#ifndef PUKAR__BUFFER_H
#define PUKAR_BUFFER_H

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include "../algo/search.h"
#include "../logger/logger.h"

typedef struct Mem_Block {
  std::shared_ptr<uint8_t[]> start;
  uint8_t* end;
  const unsigned long long size;
  const unsigned long long begin_index;

  Mem_Block(unsigned long long size, unsigned long long first_index)
      : size{size}, begin_index{first_index} {
    this->start = std::shared_ptr<uint8_t[]>(new uint8_t[size]);
  };
  Mem_Block(const Mem_Block& otherBlock) = default;
  Mem_Block operator=(const Mem_Block& otherBlock) {
    return Mem_Block(otherBlock);
  };
  uint8_t* start_ptr() const { return this->start.get(); }
} MemBlock;


class Buffer {
  static const unsigned long long int alloc_size = 200;

 private:
  std::vector<MemBlock> blocks;
  std::vector<MemBlock> merger_blocks;
  unsigned long long int block_size = Buffer::alloc_size;
  unsigned long long int block_offset = 0;
  unsigned long long int total_size = 0;
  mutable bool is_advance_pending = false;

  void add_piece(unsigned long long int size);
  unsigned long long int containing_buf_index(
      unsigned long long int const&& index) const;
  const MemBlock& containing_buf(unsigned long long int const&& index) const;

  uint8_t& at_index(unsigned long long int const&& index) const;

 public:
  explicit Buffer(unsigned long long int block_size = Buffer::alloc_size);

  [[nodiscard]] unsigned long long size() const;
  bool isFull() const;

  unsigned long long advance(unsigned long long int size);

  uint8_t* get_ensured(unsigned long long int size);
  uint8_t* get_ptr();
  uint8_t* get_rem() const;

  unsigned long long int rem() const;
  unsigned long long ensured_rem(unsigned long long int size = 0);


  friend std::ostream& operator<<(std::ostream& cout, const Buffer& buf);

  uint8_t& at(long long int const&& index) const;
  uint8_t& operator[](long long int const&& index) const;
  uint8_t& front() const;
  uint8_t& back() const;

  std::string_view get_view(unsigned long long int start_index,
                            unsigned long long int length);
};

#endif  //PUKAR__BUFFER_H
