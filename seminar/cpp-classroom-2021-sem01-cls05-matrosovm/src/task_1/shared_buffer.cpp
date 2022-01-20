#include <utility>

#include "shared_buffer.hpp"

SharedBuffer::SharedBuffer(size_t size) : data_(new char[size]()), 
	size_(size), use_count_(new size_t(1)) { } 

SharedBuffer::SharedBuffer(const SharedBuffer& shared_buffer) : data_(shared_buffer.data_), 
	size_(shared_buffer.size_), use_count_(shared_buffer.use_count_) {
	++(*use_count_);
}

void swap(SharedBuffer& sb_left, SharedBuffer& sb_right) {
	std::swap(sb_left.data_, sb_right.data_);
	std::swap(sb_left.size_, sb_right.size_);
	std::swap(sb_left.use_count_, sb_right.use_count_);
}

SharedBuffer& SharedBuffer::operator=(const SharedBuffer& shared_buffer) {
	if (this == &shared_buffer) {
		return *this;
	}
	SharedBuffer tmp(shared_buffer);
	swap(*this, tmp);
	return *this;
}

size_t SharedBuffer::getSize() const {
	return size_;
}

char* SharedBuffer::getData() const {
	return data_;
}

size_t SharedBuffer::useCount() const {
	return *use_count_;
}

SharedBuffer::~SharedBuffer() {
	if (*use_count_ > 1) {
		--(*use_count_);
		return;
	}
	delete[] data_;
	delete use_count_; 
}