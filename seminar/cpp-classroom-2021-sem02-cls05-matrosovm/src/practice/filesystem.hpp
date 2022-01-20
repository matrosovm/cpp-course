#pragma once

#include <string>

namespace filesystem {

class File {
 public:
    File(std::string path);
    std::string getPath() const;

 private:
    std::string path;
};

File createFile(std::string path);

}