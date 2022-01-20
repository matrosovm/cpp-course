#pragma once 

#include "shared_buffer.hpp"

class CharWrapper;
class LazyString;

class LazyString {
public:
	LazyString();
    LazyString(const char* c_str);
	LazyString(const LazyString& lazy_string) = default;
	LazyString& operator=(const LazyString& lazy_string);

	size_t getSize() const;
	const char* cStr() const;
	size_t useCount() const;
	char at(size_t ind) const;

	bool operator==(const LazyString& lazy_string) const;

	CharWrapper operator[](size_t ind);

    friend LazyString& operator+=(LazyString& lazy_string, const char* c_str);
    friend class CharWrapper;

    ~LazyString() = default;

private:
    SharedBuffer shared_buffer_;
};

class CharWrapper {
public:
    CharWrapper(size_t element_ind, LazyString* lazy_string);
    CharWrapper(const CharWrapper& char_wrapper) = default;

    CharWrapper& operator=(const CharWrapper& char_wrapper);
    CharWrapper& operator=(char c);

    CharWrapper& operator++();
    CharWrapper operator++(int);
    CharWrapper& operator--();
    CharWrapper operator--(int);

    operator char() const;

private:
    size_t element_ind_;
    LazyString* parent_str_;
};

LazyString operator+(const LazyString& lazy_string, const char* c_str);
