#pragma once

#include <string>

namespace cls_06 {
    class my_ostream {
    public:
        my_ostream() = default;

        virtual my_ostream& operator<<(int i) = 0;
        virtual my_ostream& operator<<(double d) = 0;
        virtual my_ostream& operator<<(const std::string& str) = 0;

        virtual ~my_ostream() = default;
    private:
    };
}