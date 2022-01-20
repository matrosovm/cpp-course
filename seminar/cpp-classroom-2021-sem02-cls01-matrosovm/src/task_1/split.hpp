#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <regex>
#include <map>


namespace cls01 {
    class StringDelimiter {
    public:
        explicit StringDelimiter(std::string pattern) : pattern_(std::move(pattern)) { }

        std::string_view find(std::string_view text, size_t pos = 0) {
            size_t edge = text.find(pattern_, pos);
            return edge == std::string_view::npos ? std::string_view{} :
                text.substr(edge, pattern_.size());
        }

    private:
        std::string pattern_;
    };


    class RegexDelimiter {
    public:
        explicit RegexDelimiter(std::regex pattern) : pattern_(std::move(pattern)) { }

        std::string_view find(std::string_view text, size_t pos = 0) {
            std::cmatch match;
            if (std::regex_search(text.data() + pos, match, pattern_)) {
                return std::string_view(text.begin() + pos + match.position(),
                        match.length());
            }
            return {};
        }

    private:
        std::regex pattern_;
    };


    template <class Delimiter>
    std::vector<std::string_view> split(std::string_view text, Delimiter&& d) {
        std::vector<std::string_view> result;
        size_t pos = 0;
        size_t part_len = 0;
        for (auto edge = d.find(text, pos); !edge.empty(); edge = d.find(text, pos)) {
            part_len = edge.begin() - (text.begin() + pos);
            result.emplace_back(text.substr(pos, part_len));
            pos += edge.size() + part_len;
        }
        part_len = text.size() - pos;
        if (part_len != 0) {
            result.emplace_back(text.substr(pos));
        }
        return result;
    }


    template <class Delimiter = StringDelimiter>
    std::vector<std::string_view> get_longest_sentences(std::string_view text,
            Delimiter d = StringDelimiter(". ")) {
        std::multimap<std::size_t, std::string_view> length_part;
        std::string_view curr_part_end = d.find(text);
        size_t offset = 0;
        while (!curr_part_end.empty()) {
            std::ptrdiff_t dist = std::distance(text.begin() + offset, curr_part_end.begin());
            length_part.insert({text.substr(offset, dist).size(),
                text.substr(offset, dist)});
            offset = std::distance(text.begin(), curr_part_end.end());
            curr_part_end = d.find(text, offset);
        }
        if (length_part.empty()) {
            return {};
        }
        auto range = length_part.equal_range(length_part.rbegin()->first);
        std::vector<std::string_view> result;
        for (auto it = range.first; it != range.second; ++it) {
            result.push_back(it->second);
        }
        return result;
    }

}