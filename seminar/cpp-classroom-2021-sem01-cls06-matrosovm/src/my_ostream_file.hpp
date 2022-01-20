#pragma once

#include <fstream>

#include "my_ostream.hpp"

namespace cls_06 {
    class my_ostream_file : public my_ostream {
    public:
        explicit my_ostream_file(const char* file_name);
        my_ostream_file(const my_ostream_file&) = delete;

        my_ostream_file& operator=(const my_ostream_file&) = delete;
        my_ostream_file& operator<<(int i) final;
        my_ostream_file& operator<<(double i) final;
        my_ostream_file& operator<<(const std::string& str) final;

        ~my_ostream_file() override;

    private:
        std::ofstream out_;
    };
}