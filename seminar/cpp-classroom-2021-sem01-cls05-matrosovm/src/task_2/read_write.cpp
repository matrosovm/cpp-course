#include <cstring>

#include "read_write.hpp"

class ConvertType {
public:
    ConvertType(const char* data, size_t size_type, size_t size_char);

    operator int();
    operator size_t();
    operator char*();

    ~ConvertType();

private:
    char* ptr_data_;
};

ConvertType::ConvertType(const char* data, size_t size_type, size_t size_char) {
    size_t size_type_data = size_type / size_char;
    ptr_data_ = new char[size_type_data];

    for (size_t i = 0; i < size_type_data; ++i) {
        ptr_data_[i] = data[i];
    }
}

ConvertType::operator int() {
    return *reinterpret_cast<int*>(ptr_data_);
}

ConvertType::operator size_t() {
    return *reinterpret_cast<size_t*>(ptr_data_);
}

ConvertType::operator char*() {
    char* tmp = new char[strlen(ptr_data_) + 1];
    strcpy(tmp, ptr_data_);
    return tmp;
}

ConvertType::~ConvertType() {
    delete[] ptr_data_;
}

const char* read(const char* data, int& out) {
    ConvertType convert_type(data, sizeof(int), sizeof(char));
    out = convert_type;
    return data + sizeof(int) / sizeof(char);
}

char* write(char* data, int in) {
    char* tmp = reinterpret_cast<char*>(&in);
    for (size_t i = 0; i < sizeof(in) / sizeof(char); ++i)
        data[i] = tmp[i];
    return data + sizeof(in) / sizeof(char);
}

const char* read(const char* data, size_t& out) {
    ConvertType convert_type(data, sizeof(size_t), sizeof(char));
    out = convert_type;
    return data + sizeof(size_t) / sizeof(char);
}

char* write(char* data, size_t in) {
    char* tmp = reinterpret_cast<char*>(&in);
    for (size_t i = 0; i < sizeof(in) / sizeof(char); ++i)
        data[i] = tmp[i];
    return data + sizeof(in) / sizeof(char);
}

const char* read(const char* data, char*& out) {
    ConvertType convert_type(data, strlen(data) + 1, sizeof(char));
    out = convert_type;
    return data + strlen(out) + 1;
}

char* write(char* data, const char* in) {
    for (size_t i = 0; i < strlen(in) + 1; ++i)
        data[i] = in[i];
    return data + strlen(in) + 1;
}

const char* read(const char* input, std::vector<int>& vec) {
    ConvertType convert_type_vec_size(input, sizeof(size_t), sizeof(char));
    input += sizeof(size_t) / sizeof(char);
    size_t vec_size = convert_type_vec_size;
    for (size_t i = 0; i < vec_size; ++i, input += sizeof(int) / sizeof(char)) {
        ConvertType tmp(input, sizeof(int), sizeof(char));
        vec.push_back(tmp);
    }
    return input;
}

char* write(char* input, const std::vector<int>& vec) {
    input = write(input, vec.size());
    for (auto item : vec) {
        input = write(input, item);
    }
    return input;
}