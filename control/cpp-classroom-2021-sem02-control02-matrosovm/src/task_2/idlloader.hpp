#pragma once

#include <memory>
#include <string>

// inspired by https://theo-penavaire.medium.com/loading-of-a-c-class-from-a-shared-library-modern-c-722d6a830a2b

namespace control_02::dlloader {
/*
** Interface for Dynamic Library Loading (DLLoader)
** API for Unix and Windows. Handling of open, close, validity-check.
*/
class IDLLoader {
 public:
  virtual ~IDLLoader() = default;

  virtual void *get_symbol(const std::string& sym_name) = 0;

  template<class T>
  T get_func(const std::string& sym_name) {
    return reinterpret_cast<T>(get_symbol(sym_name));
  }
};

}  // control_02::dlloader