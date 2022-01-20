#pragma once 

#include <string>

class StringView {
public:
	StringView(const char* c_str);
	StringView(const std::string& str);
	StringView(const char* begin_ptr, size_t size);
	StringView(const StringView& str_view);
	
	StringView& operator=(const char* c_str);
	StringView& operator=(const StringView& str_view);

	const char* data() const;
	size_t size() const;

	StringView substr(size_t begin, size_t end = std::string::npos) const;
	
	char operator[](size_t ind) const;
	
	size_t find(const StringView& str_view) const;
	size_t find(char c) const;

	bool startsWith(const StringView& str_view) const;
	bool startsWith(char c) const;
	bool endsWith(const StringView& str_view) const;
	bool endsWith(char c) const;
	
	size_t findFirstOf(const StringView& str_view, size_t begin = 0);
	size_t findFirstOf(char c, size_t begin = 0);
	size_t findFirstNotOf(const StringView& str_view, size_t begin = 0);
	size_t findFirstNotOf(char c, size_t begin = 0);	

	void removePrefix(size_t offset);

    static const size_t npos;
private:
	const char* str_;
	size_t size_;
};

inline const size_t StringView::npos = -1;