#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <cstring>
#include <regex>

namespace mp {

    class bignum {
    public:
        bignum();
        bignum(uint32_t n);
        explicit bignum(const std::string& str);
        bignum(const bignum& bn) = default;

        bignum& operator=(const bignum& bn) = default;
        bignum& operator+=(const bignum& bn);
        bignum& operator*=(const bignum& bn);
        bignum operator-(const bignum& bn) = delete;

        explicit operator uint32_t() const;
        explicit operator bool() const;

        std::string to_string() const;

    private:
        std::vector<uint32_t> buffer_;

        bignum& operator/=(uint32_t n);
        uint32_t operator%(uint32_t n) const;

        bignum mult_by_num(uint32_t n) const;

        inline static const uint64_t max_uint = 4294967296;
    };

    bignum operator+(const bignum& bn_l, const bignum& bn_r);
    bignum operator*(const bignum& bn_l, const bignum& bn_r);
    std::ostream& operator<<(std::ostream& os, const bignum& bg);
    std::istream& operator>>(std::istream& is, bignum& bg);

    class polynomial {
    public:
        explicit polynomial(const std::string& str);

        const uint32_t& at(size_t ind) const;
        uint32_t& at(size_t ind);

        template<class T>
        T operator()(const T& num) const;

    private:
        std::vector<uint32_t> data_;
    };
}

namespace mp {
    inline bignum::bignum() : buffer_(std::vector<uint32_t>(1, 0)) { }

    inline bignum::bignum(uint32_t n) : buffer_(std::vector<uint32_t>(1, n)) { }

    inline bignum::bignum(const std::string& str) {
        bignum tmp(1);
        auto digits_begin = std::find_if(str.begin(), str.end(), [] (char sym) {
            return sym != '+' && sym != '0';
        });

        for (auto it = str.rbegin(); it != std::make_reverse_iterator(digits_begin); ++it) {
            *this += tmp.mult_by_num(*it - '0');
            tmp *= 10;
        }
    }

    inline bignum& bignum::operator+=(const bignum& bn) {
        uint64_t mod = 0;
        buffer_.resize(std::max(buffer_.size(), bn.buffer_.size()));
        for (size_t i = 0; i < buffer_.size(); ++i) {
            uint64_t tmp = static_cast<uint64_t>(buffer_[i]) + mod;
            if (i < bn.buffer_.size())
                tmp += bn.buffer_[i];
            buffer_[i] = tmp % max_uint;
            mod = tmp / max_uint;
        }
        if (mod) {
            buffer_.push_back(mod);
        }
        return *this;
    }

    inline bignum bignum::mult_by_num(uint32_t n) const {
        if (!*this || !n)
            return bignum();
        bignum res(*this);
        uint64_t mod = 0;
        for (size_t i = 0; i < buffer_.size(); ++i) {
            uint64_t tmp = static_cast<uint64_t>(buffer_[i]) * n + mod;
            res.buffer_[i] = tmp % max_uint;
            mod = tmp / max_uint;
        }
        if (mod) {
            res.buffer_.push_back(mod);
        }
        return res;
    }

    inline bignum& bignum::operator*=(const bignum& bn) {
        if (!(*this) || !bn) {
            *this = 0;
            return *this;
        }
        bignum bn_l = *this;
        buffer_.resize(0);
        uint32_t end_null = 0;
        for (auto num : bn.buffer_) {
            bignum tmp = bn_l.mult_by_num(num);
            tmp.buffer_.insert(tmp.buffer_.begin(), end_null, 0);
            ++end_null;
            *this += tmp;
        }
        return *this;
    }

    inline bignum::operator uint32_t() const {
        return *buffer_.begin();
    }

    inline bignum::operator bool() const {
        return !(buffer_.size() == 1 && buffer_.back() == 0);
    }

    inline bignum operator+(const bignum& bn_l, const bignum& bn_r) {
        bignum sum(bn_l);
        return sum += bn_r;
    }

    inline bignum operator*(const bignum& bn_l, const bignum& bn_r) {
        bignum compos(bn_l);
        return compos *= bn_r;
    }

    inline uint32_t bignum::operator%(uint32_t n) const {
        uint64_t mod = 0;
        uint64_t last_numeral = 1;
        for (const auto num : buffer_) {
            mod += (num % 10) * last_numeral;
            mod %= n;
            last_numeral *= 6; //2^32 = 4294967296
        }
        return mod;
    }

    inline bignum& bignum::operator/=(uint32_t n) {
        std::vector<uint32_t> div;
        uint64_t mod = 0;
        for (auto it = buffer_.rbegin(); it != buffer_.rend(); ++it) {
            uint64_t tmp = *it + mod * max_uint;
            div.push_back(tmp / n);
            mod = tmp % n;
        }
        std::vector<uint32_t> vec(div.rbegin(), div.rend());
        swap(buffer_, vec);
        return *this;
    }

    inline std::string bignum::to_string() const {
        std::string str_num;
        bignum tmp(*this);
        while (tmp) {
            str_num.push_back(tmp % 10 + '0');
            tmp /= 10;
            if (tmp.buffer_.size() > 1 && !tmp.buffer_.back())
                tmp.buffer_.pop_back();
        }
        if (str_num.empty())
            return "0";
        return std::string(str_num.rbegin(), str_num.rend());
    }

    inline std::ostream& operator<<(std::ostream& os, const bignum& bg) {
        os << bg.to_string();
        return os;
    }

    inline std::istream& operator>>(std::istream& is, bignum& bg) {
        std::string str_num;
        is >> str_num;
        bg = bignum(str_num);
        return is;
    }
}

namespace mp {
    inline polynomial::polynomial(const std::string& str) {
        std::regex reg(R"(x\^(\d+))");
        std::smatch matches;
        size_t max_degree = 0;

        for (auto it = std::sregex_iterator(str.begin(), str.end(), reg);
            it != std::sregex_iterator(); ++it) {
            max_degree = std::max(max_degree, std::stoul(it->str().substr(2)));
        }
        data_.resize(max_degree + 1);

        reg = std::regex(R"((\d+)?|(\*)?x\^|(\d*))");
        uint32_t factor = 1;
        bool is_factor = true;
        for (auto it = std::sregex_iterator(str.begin(), str.end(), reg);
            it != std::sregex_iterator(); ++it) {
            if (it->str().empty())
                continue;

            if (it->str() == "*x^" || it->str() == "x^") {
                is_factor = false;
                continue;
            }
            if (!is_factor) {
                data_[std::stoul(it->str())] = factor;
                factor = 1;
                is_factor = true;
            }
            else {
                factor = std::stoul(it->str());
            }
        }
    }

    inline const uint32_t& polynomial::at(size_t ind) const {
        return data_.at(ind);
    }

    inline uint32_t& polynomial::at(size_t ind) {
        if (ind >= data_.size()) {
            data_.resize(ind + 1);
        }
        return data_.at(ind);
    }

    template<class T>
    inline T polynomial::operator()(const T& num) const {
        T res(*data_.rbegin());
        for (auto it = std::next(data_.rbegin()); it != data_.rend(); ++it) {
            res = num * res + *it;
        }
        return res;
    }

}