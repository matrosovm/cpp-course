#pragma once

#include <any>
#include <map>
#include <unordered_map>
#include <string>
#include <tuple>
#include <variant>
#include <exception>
#include <functional>


namespace utils {

    class no_key_exception : std::exception {
        const char* what() const noexcept override {
            return "key exception";
        }
    };

    class invalid_type_exception : std::exception {
        const char* what() const noexcept override {
            return "invalid type";
        }
    };

    class dict_t {
    
    public:
        dict_t() = default;

        template <class T, std::enable_if_t<std::is_copy_constructible_v<T>, bool> = true>
        dict_t(const T& tmp) noexcept(std::is_nothrow_constructible_v<T>) {
            data_.emplace("fake", std::move(tmp));
        }

        dict_t(std::initializer_list<std::pair<std::string, dict_t>> list) {
            std::unordered_map<std::string, helper> tmp;
            try {
                for (auto& item : list) {
                    if (!item.second.data_.contains("fake")) {
                        tmp.emplace(item.first, helper(item.second));
                    } else {
                        tmp.emplace(item.first, helper(item.second.data_.at("fake")));
                    }
                }
            } catch (const std::exception&) {
                return;
            }
            data_.swap(tmp);
        }

        dict_t(const dict_t& other) {
            std::unordered_map<std::string, helper> tmp;
            try {
                tmp = other.data_;
            } catch (const std::exception&) {
                return;
            }
            data_.swap(tmp);
        }

        dict_t(dict_t&& other)  noexcept : data_(std::move(other.data_)) { }


        dict_t& operator=(const dict_t& other) {
            if (this == &other) {
                return *this;
            }
            std::unordered_map<std::string, helper> tmp;
            try {
                tmp = other.data_;
            } catch (const std::exception&) {
                return *this;
            }
            data_.swap(tmp);
            return *this;
        }

        dict_t& operator=(dict_t&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            data_ = std::move(other.data_);
            return *this;
        }


        template <class T>
        bool put(const std::string& key, T&& value) {
            try {
                return data_.emplace(key, std::forward<T>(value)).second;
            } catch (const std::exception&) {
                return false;
            }
        }

        template <class T>
        T& get(const std::string& key) {
            auto it = data_.find(key);
            if (it == data_.end()) {
                throw no_key_exception();
            } else if (it->second.storage.type() != typeid(T)) {
                throw invalid_type_exception();
            }
            return std::any_cast<T&>(data_.at(key).storage);
        }

        template <class T>
        const T& get(const std::string& key) const {
            auto it = data_.find(key);
            if (it == data_.end()) {
                throw no_key_exception();
            } else if (it->second.storage.type() != typeid(T)) {
                throw invalid_type_exception();
            }
            return std::any_cast<const T&>(data_.at(key).storage);
        }

        template <class T>
        T* get_ptr(const std::string& key) {
            auto it = data_.find(key);
            if (it == data_.end() || it->second.storage.type() != typeid(T)) {
                return nullptr;
            }
            return &std::any_cast<T&>(data_.at(key).storage);
        }

        template <class T>
        const T* get_ptr(const std::string& key) const {
            auto it = data_.find(key);
            if (it == data_.end() || it->second.storage.type() != typeid(T)) {
                return nullptr;
            }
            return &std::any_cast<const T&>(data_.at(key).storage);
        }


        bool operator==(const dict_t& other) const {
            return data_ == other.data_;
        }

        bool empty() const {
            return data_.empty();
        }

        bool remove(const std::string& key) {
            return data_.erase(key);
        }

        bool contains(const std::string& key) const {
            return data_.contains(key);
        }

        void clear() {
            data_.clear();
        }

        bool is_dict(const std::string& key) const {
            if (!data_.contains(key)) {
                return false;
            }
            return data_.at(key).storage.type() == typeid(dict_t);
        }

        size_t size() const {
            return data_.size();
        }


        auto begin() const noexcept {
            return data_.begin();
        }

        auto end() const noexcept {
            return data_.end();
        }


        ~dict_t() = default;
        
    private:

        struct helper {
            std::any storage;
            std::function<bool (const std::any&, const std::any&)> comparator;

            template <class T>
            explicit helper(T value) : storage(std::move(value)),
                comparator ([](const std::any& lhs, const std::any& rhs) -> bool {
                    if (lhs.type() != rhs.type()) {
                        return false;
                    }
                    return std::any_cast<const T&>(lhs) == std::any_cast<const T&>(rhs);
                }) { }


            bool operator==(const helper& other) const {
                return comparator(storage, other.storage);
            }

            helper(const helper& other) = default;
            helper(helper&& other) = default;

        };

        std::unordered_map<std::string, helper> data_{};

    };

}