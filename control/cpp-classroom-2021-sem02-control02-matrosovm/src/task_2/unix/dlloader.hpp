#pragma once

#include <iostream>
#include <dlfcn.h>
#include "idlloader.hpp"

namespace control_02::dlloader {
class DLLoader : public IDLLoader {
 private:
  void* handle_;
  std::string path_to_lib_;

 public:
  DLLoader(std::string const &path_to_lib) :
          handle_(nullptr), path_to_lib_(path_to_lib) {
    if (!(handle_ = dlopen(path_to_lib_.c_str(), RTLD_NOW | RTLD_LAZY))) {
      std::cerr << dlerror() << std::endl;
      throw std::runtime_error(dlerror());
    }
  }

  ~DLLoader() {
    if (dlclose(handle_) != 0) {
      std::cerr << dlerror() << std::endl;
    }
  }

  void* get_symbol(const std::string& symname) override {
    return dlsym(handle_, symname.c_str());
  }
};
}
