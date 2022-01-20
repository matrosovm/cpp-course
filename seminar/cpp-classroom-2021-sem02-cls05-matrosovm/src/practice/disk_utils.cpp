#include "disk_utils.hpp"

namespace disk_utils {

size_t du(const std::string& path) {
    return 4;
}

size_t du(const filesystem::File& file) {
    return file.getPath().size();
}

}