#include "bad_lib.hpp"

namespace bad_lib {

std::string ToString(const Gender gender) {
    switch (gender) {
        case kFemale: return "female";
        case kMale: return "male";
    }
}

Cat bornACat(const Gender gender) {
    return Cat{gender, 0, 0};
}

Cat reincarnateACat(Cat cat) {
    if (cat.live_index >= 8) {
        throw 4;
    }
    return Cat{cat.gender, 0, cat.live_index + 1};
}

std::string ToStringInCpp(const Cat& cat) {
    std::stringstream ss;
    ss << "size of a cat is " << sizeof(cat)
       << ", gender is " << ToString(cat.gender)
       << " in the age of " << cat.age
       << " lives his/her " << cat.live_index << "th live";
    return ss.str();
}

Cat anotherYearOfLive(Cat cat) {
    return Cat{cat.gender, cat.age + 1, cat.live_index};
}

}