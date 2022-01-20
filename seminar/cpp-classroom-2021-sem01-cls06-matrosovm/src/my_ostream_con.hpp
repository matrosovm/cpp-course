#pragma once

#include "my_ostream.hpp"

namespace cls_06 {
    class my_ostream_con : public my_ostream {
    public:
        my_ostream_con& operator<<(int i) final;
        my_ostream_con& operator<<(double d) final;
        my_ostream_con& operator<<(const std::string& str) final;

    private:
    };
}