#include "my_ostream_con.hpp"
#include "my_ostream_combo.hpp"

namespace cls_06 {
    my_ostream_combo::my_ostream_combo(my_ostream* os1, my_ostream* os2) : os1_(os1), os2_(os2) { }

    my_ostream_combo& my_ostream_combo::operator<<(int i) {
        *os1_ << i;
        *os2_ << i;
        return *this;
    }

    my_ostream_combo& my_ostream_combo::operator<<(double d) {
        *os1_ << d;
        *os2_ << d;
        return *this;
    }

    my_ostream_combo& my_ostream_combo::operator<<(const std::string& str) {
        *os1_ << str;
        *os2_ << str;
        return *this;
    }

    my_ostream_combo::~my_ostream_combo() {
        delete os1_;
        delete os2_;
    }
}