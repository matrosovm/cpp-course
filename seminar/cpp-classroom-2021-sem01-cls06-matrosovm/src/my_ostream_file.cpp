#include "my_ostream_file.hpp"

namespace cls_06 {
    my_ostream_file::my_ostream_file(const char* file_name) : out_(file_name) {
        if (!out_.is_open())
            throw std::runtime_error("Could not open file");
    }

    my_ostream_file& my_ostream_file::operator<<(int i) {
        out_ << i;
        return *this;
    }

    my_ostream_file& my_ostream_file::operator<<(double d) {
        out_ << d;
        return *this;
    }

    my_ostream_file& my_ostream_file::operator<<(const std::string& str) {
        out_ << str;
        return *this;
    }

    my_ostream_file::~my_ostream_file() {
        out_.close();
    }
}