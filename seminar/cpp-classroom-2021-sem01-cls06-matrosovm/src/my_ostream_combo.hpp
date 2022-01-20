#pragma once

#include "my_ostream.hpp"

namespace cls_06 {
    class my_ostream_combo : public my_ostream {
    public:
        my_ostream_combo(my_ostream* os1, my_ostream* os2);
        my_ostream_combo(const my_ostream_combo& moc) = delete;

        my_ostream_combo& operator<<(int i) final;
        my_ostream_combo& operator<<(double d) final;
        my_ostream_combo& operator<<(const std::string& str) final;
        my_ostream_combo& operator=(const my_ostream_combo& mos) = delete;

        ~my_ostream_combo() override;

    private:
        my_ostream* os1_;
        my_ostream* os2_;
    };
}