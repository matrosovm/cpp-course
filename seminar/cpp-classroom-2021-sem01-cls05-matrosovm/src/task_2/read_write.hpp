#pragma once

#include <cstddef>
#include <vector>

const char* read(const char* data, int& out);
char* write(char* data, int in);

const char* read(const char* data, size_t& out);
char* write(char* data, size_t in);

const char* read(const char* data, char*& out);
char* write(char* data, const char* in);

const char* read(const char* input, std::vector<int>& vec);
char* write(char* input, const std::vector<int>& vec);