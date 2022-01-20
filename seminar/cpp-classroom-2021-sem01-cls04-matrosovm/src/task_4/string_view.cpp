#include <cstring>

#include "string_view.hpp"

StringView::StringView(const char* c_str) : str_(c_str), size_(strlen(c_str)) { }

StringView::StringView(const std::string& str) : str_(str.data()), size_(str.size()) { }

StringView::StringView(const char* begin_ptr, size_t size) : str_(begin_ptr), size_(size) { }

StringView::StringView(const StringView& str_view) : str_(str_view.str_), size_(str_view.size_)	{ }

StringView& StringView::operator=(const char* c_str) {
	str_ = c_str;
	size_ = strlen(c_str);
	return *this;
}

StringView& StringView::operator=(const StringView& str_view) {
	str_ = str_view.str_;
	size_ = str_view.size_;
	return *this;
}

const char* StringView::data() const {
	return str_;
}

size_t StringView::size() const {
	return size_;
}

StringView StringView::substr(size_t begin, size_t end) const {
	if (end > size_)
		end = size_;

	if (end == StringView::npos)
		return StringView(&str_[begin]);
	return StringView(&str_[begin], end - begin);
}

char StringView::operator[](size_t ind) const {
	return str_[ind]; 
}

size_t StringView::find(const StringView& str_view) const {
	const char* c1 = str_;
	const char* c2 = str_view.str_;
	const char* edge = str_;

	size_t offset1 = 0;
	size_t offset2 = 0;

	while (str_view.size_ + offset1 <= size_) {
		while (offset2 < str_view.size_ && *c1 == *c2) {
            ++c1;
            ++c2;
            ++offset2;
        }
		if (offset2 == str_view.size_)
			return offset1;
		
		++offset1;
		offset2 = 0;
		++edge;
		c1 = edge;
		c2 = str_view.str_;
	}

	return StringView::npos;
}

size_t StringView::find(char c) const {
	return find(StringView(&c, 1));
}

bool StringView::startsWith(const StringView& str_view) const {
	const char* c1 = str_;
	const char* c2 = str_view.str_;
	size_t offset2 = 0;
	
	while (offset2 < str_view.size_ && *c1 == *c2)
		++c1, ++c2, ++offset2;
	if (offset2 == str_view.size_)
		return true;
	return false;
}

bool StringView::startsWith(char c) const {
	return startsWith(StringView(&c, 1));
}

bool StringView::endsWith(const StringView& str_view) const {
	StringView tmp(substr(size_ - str_view.size_));
	return tmp.startsWith(str_view);
}

bool StringView::endsWith(char c) const {
	return endsWith(StringView(&c, 1));
}

size_t StringView::findFirstOf(const StringView& str_view, size_t begin) {
	const char* c1 = &str_[begin];
	const char* c2 = str_view.str_;
	size_t offset1 = begin;
	size_t offset2 = 0;

	while (offset1 < size_) {
		while (offset2 < str_view.size_) {
			if (*c1 == *c2)
				return offset1;
			++c2;
			++offset2;
		}
		++offset1;
		++c1;
		c2 = str_view.str_;
		offset2 = 0;
	}

	return StringView::npos;
}

size_t StringView::findFirstOf(char c, size_t begin) {
	return findFirstOf(StringView(&c, 1), begin);
}

size_t StringView::findFirstNotOf(const StringView& str_view, size_t begin) {
	const char* c1 = &str_[begin];
	const char* c2 = str_view.str_;
	size_t offset1 = begin;
	size_t offset2 = 0;

	while (offset1 < size_) {
		while (offset2 < str_view.size_) {
			if (*c1 == *c2)
				break;
			++c2;
			++offset2;
		}
		if (offset2 == str_view.size_)
			return offset1;

		++offset1;
		++c1;
		c2 = str_view.str_;
		offset2 = 0;
	}

	return StringView::npos;		
}

size_t StringView::findFirstNotOf(char c, size_t begin) {
	return findFirstNotOf(StringView(&c, 1), begin);
}

void StringView::removePrefix(size_t offset) {
	if (offset >= size_)
		offset = size_ - 1;

	str_ = &str_[offset];
	size_ -= offset;
}
