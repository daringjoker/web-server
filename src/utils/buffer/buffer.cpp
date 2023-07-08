#include "./buffer.h"

void Buffer::add_piece(unsigned long long int size) {
  if (!blocks.empty()) {
    auto& current_piece = blocks.back();
    current_piece.end = current_piece.start_ptr() + block_offset;
  }
  blocks.emplace_back(size, total_size);
  block_offset = 0;
}

unsigned long long int Buffer::containing_buf_index(
    unsigned long long int const&& index) const {
  const auto probable_index = lessThanOrEqual(
      blocks, index,
      [](MemBlock p) -> unsigned long long int { return p.begin_index; });
  if (probable_index < 0) {
    throw std::out_of_range("The index could not be found ");
  }
  return probable_index;
}

const MemBlock& Buffer::containing_buf(
    unsigned long long int const&& index) const {
  const auto probable_index = containing_buf_index(std::move(index));
  return blocks.at(probable_index);
}

uint8_t& Buffer::at_index(unsigned long long int const&& index) const {
  const MemBlock& probable_piece = containing_buf(std::move(index));

  if (index >= total_size || blocks.empty()) {
    throw std::out_of_range("Current Access is out of range");
  }
  const unsigned long long computed_offset = index - probable_piece.begin_index;

  if (&probable_piece == &blocks.back() && computed_offset >= block_offset) {
    throw std::out_of_range(
        "Current Access is out of range. This should not have happened!");
  }

  return probable_piece.start_ptr()[computed_offset];
}


Buffer::Buffer(unsigned long long int block_size) {
  this->block_size = block_size;
  add_piece(block_size);
}

unsigned long long int Buffer::rem() const {
  return blocks.back().size - block_offset;
}

[[nodiscard]] unsigned long long Buffer::size() const {
  return total_size;
}

unsigned long long Buffer::advance(unsigned long long int size) {
  if (size > blocks.back().size) {
    throw std::invalid_argument(
        "could not advance pointer by more than the size; this might signify "
        "a buffer overflow somewhere");
  }
  this->block_offset += size;
  this->total_size += size;
  this->is_advance_pending = false;
  return this->block_offset;
}

uint8_t* Buffer::get_ensured(unsigned long long int size) {
  if (this->is_advance_pending) {
    throw std::domain_error(
        "Must advance the pointer before getting the new pointer");
  }

  this->is_advance_pending = true;
  const auto remaining = rem();
  if (remaining > size) {
    return this->blocks.back().start_ptr() + block_offset;
  } else {
    add_piece(size);
    return this->blocks.back().start_ptr();
  }
}

unsigned long long Buffer::ensured_rem(unsigned long long int size) {
  if (size == 0) {
    size = this->block_size;
  }

  if (rem() == 0) {
    add_piece(size);
  }

  return rem();
}

uint8_t* Buffer::get_ptr() {
  if (isFull()) {
    add_piece(this->block_size);
  }
  return get_rem();
}

bool Buffer::isFull() const {
  return rem() == 0;
}

uint8_t* Buffer::get_rem() const {
  if (this->is_advance_pending) {
    throw std::domain_error(
        "Must advance the pointer before getting the new pointer");
  }


  if (this->rem() == 0) {
    throw std::invalid_argument(
        "No remaining Buffer in current chunk! you must check the remaining "
        "size before calling get_rem");
  }

  this->is_advance_pending = true;
  return this->blocks.back().start_ptr() + block_offset;
}

uint8_t& Buffer::operator[](long long int const&& index) const {
  return this->at(std::move(index));
}

std::ostream& operator<<(std::ostream& cout, const Buffer& buf) {
  for (const MemBlock& block : buf.blocks) {
    unsigned long long int len = &block == &buf.blocks.back()
                                     ? buf.block_offset
                                     : block.end - block.start_ptr();
    cout.write((char*)block.start_ptr(), len);
  }
  return cout;
}

uint8_t& Buffer::at(long long int const&& index) const {
  return index < 0 ? at_index(total_size + index) : at_index(index);
}

uint8_t& Buffer::front() const {
  return blocks[0].start_ptr()[0];
}

uint8_t& Buffer::back() const {
  return blocks.back().start_ptr()[block_offset - 1];
}

std::string_view Buffer::get_view(unsigned long long int start_index,
                                  unsigned long long int length) {
  const auto& start_buf_index = containing_buf_index(std::move(start_index));
  const auto& end_buf_index = containing_buf_index(start_index + length - 1);

  if (start_buf_index == end_buf_index) {
    const auto& start_buf = blocks.at(start_buf_index);
    return std::string_view(
        (char*)start_buf.start_ptr() + (start_index - start_buf.begin_index),
        length);
  }
  for (auto& merge_block : merger_blocks) {
    if (start_index >= merge_block.begin_index &&
        start_index + length - 1 <=
            merge_block.begin_index +
                (merge_block.end - merge_block.start_ptr())) {
      // std::cout << "Found an old merge block using that again\n";
      return std::string_view((char*)merge_block.start_ptr() +
                                  (start_index - merge_block.begin_index),
                              length);
    }
  }

  merger_blocks.emplace_back(length, start_index);
  auto& block = merger_blocks.back();
  char* dest_start = (char*)block.start_ptr();
  for (unsigned long long int i = start_buf_index; i <= end_buf_index; i++) {
    auto* copy_source =
        (i == start_buf_index)
            ? blocks[i].start_ptr() + (start_index - blocks[i].begin_index)
            : blocks[i].start_ptr();
    unsigned long long int copy_size =
        (i == end_buf_index) ? (start_index + length - blocks[i].begin_index)
                             : blocks[i].end - copy_source;
    // printf("Copying %llu bytes from  %p to %p\n", copy_size,
    // (void*)copy_source, (void*)dest_start);
    memcpy(dest_start, copy_source, copy_size);

    dest_start += copy_size;
  }
  block.end = (uint8_t*)dest_start;
  return std::string_view((char*)block.start_ptr(), length);
}
