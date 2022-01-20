#pragma once

#include <cstddef>

class SharedBuffer {
public:
	explicit SharedBuffer(size_t size);
	SharedBuffer(const SharedBuffer& shared_buffer);

	SharedBuffer& operator=(const SharedBuffer& shared_buffer);

	size_t getSize() const;
	char* getData() const;
	size_t useCount() const;

	friend void swap(SharedBuffer& sb_left, SharedBuffer& sb_right);

	~SharedBuffer();

private:
	char* data_;
	size_t size_;
	size_t* use_count_;
};