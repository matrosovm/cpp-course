#include "filesystem.hpp"

namespace filesystem {

File::File(std::string s) : path(std::move(s)) {}

std::string File::getPath() const {
    return path;
}

File createFile(std::string path) {
    return File(std::move(path));
}

}