#pragma once

#include <cstdint>
#include <string>
#include "filesystem.hpp"

namespace disk_utils {

size_t du(const std::string& path);

size_t du(const filesystem::File& file);

}