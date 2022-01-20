extern "C" {
#include "calculate_impl/Calc.h"
}

#include <cstring>
#include "calculate.hpp"

namespace detail {
void handle_error() {
  g_error = 0;
  throw calculate_exception{};
}
}

double calculate(const std::string& expr) {
  InitDefCharacters();
  InitConstants();

  leaf_t* root = (leaf_t*)malloc(sizeof(leaf_t));
  InitLeaf(root);
  char* s1 = (char*)malloc(expr.length() + 1);
  strcpy(s1, expr.c_str());

  double result = 0;
  if (!BadString(s1))
  {
    BuildTree(s1, root);
    result = GetResult(root);
    IsNan(result);
    IsOverFlow(result);
    DestroyTree(root);
  }
  else  // bad string or comment
  {
    free(root);
    if (s1)
      free(s1);
  }

  if (g_error) {
    detail::handle_error();
  }
  return result;
}