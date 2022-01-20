#pragma once

#include <map>
#include <vector>

#include "dict.hpp"

namespace utils {

    namespace details {
        template <class T>
        using simple_condition_tr = std::enable_if_t<std::is_arithmetic_v<std::decay_t<T>> ||
                                           std::is_same_v<std::decay_t<T>, std::string>, bool>;

        template <class T>
        using simple_condition_fl = std::enable_if_t<!std::is_arithmetic_v<std::decay_t<T>> &&
                                           !std::is_same_v<std::decay_t<T>, std::string>, bool>;
    }

    void read(const dict_t& dict, dict_t& value) {
        value = dict;
    }

    template<class T>
    void read(const dict_t& dict, std::map<std::string, T>& value) {
        for (const auto& [key, item] : dict) {
            if constexpr (std::is_arithmetic_v<std::decay_t<T>> || std::is_same_v<std::decay_t<T>, std::string>) {
                value[key] = std::any_cast<T>(item.storage);
            } else {
                read(dict.get<dict_t>(key), value[key]);
            }
        }
    }

    template<class T, details::simple_condition_tr<T> = true>
    void read(const dict_t& dict, std::vector<T>& value) {
        value.resize(dict.size());
        for (const auto& [key, item] : dict) {
            value.at(std::stoi(key)) = std::any_cast<T>(item.storage);
        }
    }

    template<class T, details::simple_condition_fl<T> = true>
    void read(const dict_t& dict, std::vector<T>& value) {
        value.resize(dict.size());
        for (const auto& [key, item] : dict) {
            read(std::any_cast<dict_t>(item.storage), value.at(std::stoi(key)));
        }
    }

    void write(dict_t& dict, const dict_t& value) {
        dict = value;
    }

    template<class T, details::simple_condition_tr<T> = true>
    void write(dict_t& dict, const std::map<std::string, T>& value) {
        for (const auto& [key, item] : value) {
            dict.put(key, item);
        }
    }

    template<class T, details::simple_condition_fl<T> = true>
    void write(dict_t& dict, const std::map<std::string, T>& value) {
        for (const auto& [key, item] : value) {
            dict_t tmp;
            write(tmp, item);
            dict.put(key, tmp);
        }
    }

    template<class T, details::simple_condition_tr<T> = true>
    void write(dict_t& dict, const std::vector<T>& value) {
        for (std::size_t i = 0; i < value.size(); ++i) {
            dict.put(std::to_string(i), value.at(i));
        }
    }

    template<class T, details::simple_condition_fl<T> = true>
    void write(dict_t& dict, const std::vector<T>& value) {
        for (std::size_t i = 0; i < value.size(); ++i) {
            dict_t tmp;
            write(tmp, value.at(i));
            dict.put(std::to_string(i), tmp);
        }
    }

}