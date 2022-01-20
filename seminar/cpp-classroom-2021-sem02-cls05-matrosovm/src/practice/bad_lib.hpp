#pragma once

#include <sstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

namespace bad_lib {

enum Gender {
    kFemale,
    kMale,
};

std::string ToString(const Gender gender);

struct __attribute__((packed)) Cat {
    Gender gender;
    long age;
    long live_index;
};

Cat bornACat(const Gender gender);

Cat anotherYearOfLive(Cat cat);

Cat reincarnateACat(Cat cat);

inline std::string ToStringInHeader(const Cat& cat) {
    std::stringstream ss;
    ss << "size of a cat is " << sizeof(cat)
       << ", gender is " << ToString(cat.gender)
       << " in the age of " << cat.age
       << " lives his/her " << cat.live_index << "th live";
    return ss.str();
}

std::string ToStringInCpp(const Cat& cat);

}