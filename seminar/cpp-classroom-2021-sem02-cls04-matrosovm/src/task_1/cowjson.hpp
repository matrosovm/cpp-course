#pragma once

#include <exception>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <memory>
#include <type_traits>
#include <fstream>
#include <iomanip>

namespace cls_04 {

    class CowJsonException : public std::exception {
    public:
        explicit CowJsonException(const std::string& msg) noexcept: msg_(msg) {}

        const char* what() const noexcept override {
            return msg_.c_str();
        }

    private:
        std::string msg_;
    };

    class CowJsonDeserializeException : public CowJsonException {
        using CowJsonException::CowJsonException;
    };

    class CowJsonBadAccess : public CowJsonException {
        using CowJsonException::CowJsonException;
    };

    template <template <class Key, class Value, class...> class Map = std::map,
        template <class Value, class...> class Array = std::vector>
    class CowJson {

    public:
        using dict = Map<std::string, CowJson>;
        using array = Array<CowJson>;
        using type = std::variant<std::nullptr_t, bool, int, double, std::string, dict, array>;

        template <class T>
        static constexpr bool cond =
            std::is_constructible_v<std::nullptr_t, T> ||
            std::is_constructible_v<bool, T> ||
            std::is_constructible_v<int, T> ||
            std::is_constructible_v<double, T> ||
            std::is_constructible_v<std::string, T> ||
            std::is_constructible_v<dict, T> ||
            std::is_constructible_v<array, T>;


        CowJson() : data_(std::make_shared<type>(nullptr)) {}

        template <class T, std::enable_if_t<CowJson::cond<T>, bool> = true>
        CowJson(T&& t) : data_(std::make_shared<type>(std::forward<T>(t))) {}

        CowJson(const CowJson& cj) : data_(cj.data_) {}

        CowJson(CowJson&& cj) noexcept: data_(std::move(cj.data_)) {}

        template <class T, std::enable_if_t<CowJson::cond<T>, bool> = true>
        CowJson& operator=(T&& t) {
            data_.template reset(new type{std::forward<T>(t)});
            return *this;
        }

        CowJson& operator=(const CowJson& cj) {
            if (this == &cj) {
                return *this;
            }
            data_ = cj.data_;
            return *this;
        }

        CowJson& operator=(CowJson&& cj) noexcept {
            if (this == &cj) {
                return *this;
            }
            data_ = std::move(cj.data_);
            return *this;
        }

        template <class T, std::enable_if_t<CowJson::cond<T>, bool> = true>
        bool is() const {
            return std::holds_alternative<T>(*data_);
        }

        template <class T>
        bool operator==(const T& t) const {
            if (is<T>()) {
                return std::get<T>(*data_) == t;
            }
            return false;
        }

        bool operator==(const CowJson& cj) const {
            return *data_ == *cj.data_;
        }

        template <class T, std::enable_if_t<CowJson::cond<T> && !std::is_const_v<T>, bool> = true>
        std::remove_cvref_t<T>& get() {
            using ncnrt = std::remove_cvref_t<T>;
            if (data_.use_count() == 1) {
                if (is<ncnrt>()) {
                    return std::get<ncnrt>(*data_);
                }
            }
            data_.template reset(new type{*data_});
            return std::get<ncnrt>(*data_);
        }

        template <class T, std::enable_if_t<CowJson::cond<T>, bool> = true>
        const std::remove_cvref_t<T>& get() const {
            using ncnrt = std::remove_cvref_t<T>;
            return std::get<ncnrt>(*data_);
        }

        template <class T, std::enable_if_t<CowJson::cond<T>, bool> = true>
        const std::remove_cvref_t<T>& cget() const {
            using ncnrt = std::remove_cvref_t<T>;
            return std::get<ncnrt>(*data_);
        }

        bool empty() const {
            return is<std::nullptr_t>();
        }

        void dump(std::ostream& out) const {
            if (is<std::nullptr_t>()) {
                out << "null";
            } else if (is<bool>()) {
                out << std::boolalpha << cget<bool>();
            } else if (is<int>()) {
                out << cget<int>();
            } else if (is<double>()) {
                out << std::fixed << std::setprecision(6) << cget<double>();
            } else if(is<std::string>()) {
                out << "\"" << cget<std::string>() << "\"";
            } else if (is<array>()) {
                out << "[";
                size_t size = std::size(cget<array>());
                for (size_t i = 0; i < size; ++i) {
                    cget<array>()[i].dump(out);
                    if (i == size - 1) {
                        continue;
                    }
                    out << ",";
                }
                out << "]";
            } else if (is<dict>()) {
                out << "{";
                size_t i = 0;
                for (const auto& item : cget<dict>()) {
                    out << "\"" << item.first << "\":";
                    item.second.dump(out);
                    if (i++ != cget<dict>().size() - 1) {
                        out << ',';
                    }
                }
                out << "}";
            }
        }

        static CowJson restore(std::istream &in) {
            CowJson cow_json;
            char curr_sym = static_cast<char>(in.peek());
            if (curr_sym == 'n') {
                cow_json = read_nullptr(in);
            } else if (curr_sym == 't' || curr_sym == 'f') {
                cow_json = read_bool(in);
            } else if (std::isdigit(curr_sym)) {
                cow_json = read_number(in);
            } else if (curr_sym == '\"') {
                cow_json = read_string(in);
            } else if (curr_sym == '[') {
                cow_json = array{};
                for (char s = in.get(); s != ']'; s = static_cast<char>(in.peek())) {
                    if (s == ',') {
                        s = in.get();
                        continue;
                    }
                    CowJson elem = restore(in);
                    cow_json.template get<array>().push_back(elem);
                }
                in.get();
            } else if (curr_sym == '{') {
                cow_json = dict{};
                for (char s = in.get(); s != '}'; s = static_cast<char>(in.peek())) {
                    if (s == ',') {
                        s = in.get();
                        continue;
                    }
                    CowJson key = read_string(in);
                    in.get();
                    CowJson value = restore(in);
                    cow_json.template get<dict>()[key.template get<std::string>()] = value;
                }
                in.get();
            } else if (curr_sym != ']' && curr_sym != '}') {
                throw CowJsonDeserializeException("unknown symbol");
            }
            return cow_json;
        }

        ~CowJson() = default;

    private:
        std::shared_ptr<type> data_;

        static CowJson read_number(std::istream &in) {
            int in_part = 0;
            double frac_part = 0;
            in >> in_part;
            if (in.peek() == '.') {
                in >> frac_part;
            }
            return frac_part == 0 ? CowJson{in_part} : CowJson{in_part + frac_part};
        }

        static CowJson read_bool(std::istream &in) {
            std::string bool_ex{};
            char s;
            for (s = in.get(); s != 'e' && s != in.eof(); in >> s) {
                bool_ex += s;
            }
            if (s == in.eof() || (bool_ex != "tru" && bool_ex != "fals")) {
                throw CowJsonDeserializeException("missing bracket");
            }
            return CowJson{bool_ex == "tru"};
        }

        static CowJson read_nullptr(std::istream &in) {
            std::string null_string = "null";
            for (auto c : null_string) {
                if (in.get() != c) {
                    throw CowJsonDeserializeException("wrong symbol in null");
                }
            }
            return CowJson{nullptr};
        }

        static CowJson read_string(std::istream &in) {
            std::string res{};
            char s;
            in.get();
            for (s = in.get(); s != '\"' && s != in.eof(); in >> s) {
                res += s;
            }
            if (s == in.eof()) {
                throw CowJsonDeserializeException("missing bracket");
            }
            return CowJson{res};
        }

    };

}  // namespace cls_04