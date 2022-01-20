#include <iostream>

#include "my_ostream_con.hpp"

namespace cls_06 {
    my_ostream_con& my_ostream_con::operator<<(int i) {
        std::cout << i;
        return *this;
    }

    my_ostream_con& my_ostream_con::operator<<(double d) {
        std::cout << d;
        return *this;
    }

    my_ostream_con& my_ostream_con::operator<<(const std::string& str) {
        std::cout << str;
        return *this;
    }
}
