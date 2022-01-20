#include <cassert>
#include <iostream>
#include <fstream>
#include <type_traits>
#include "my_ostream.hpp"
#include "my_ostream_file.hpp"

namespace {

std::string read_file_content(const std::string &name) {
  std::fstream file{name};
  return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

static_assert(std::is_abstract<cls_06::my_ostream>::value, "my_ostream should be abstract class!");
static_assert(!std::is_copy_constructible<cls_06::my_ostream_file>::value,
              "You shouldn't be able to copy my_ostream_file!");
static_assert(!std::is_copy_assignable<cls_06::my_ostream_file>::value,
              "You shouldn't be able to copy my_ostream_file!");

void test_file_stream() {
  const char *file_name = "tmp.txt";
  {
    std::remove(file_name);
    {
      cls_06::my_ostream_file file_stream{file_name};
      cls_06::my_ostream &ref = file_stream;
      ref << "Hello there!";
    }

    assert(read_file_content(file_name) == "Hello there!");
  }

  {
    std::remove(file_name);
    {
      cls_06::my_ostream_file file_stream{file_name};
      cls_06::my_ostream &ref = file_stream;
      ref << 1024;
    }

    assert(read_file_content(file_name) == "1024");
  }

  {
    std::remove(file_name);
    {
      cls_06::my_ostream_file file_stream{file_name};
      cls_06::my_ostream &ref = file_stream;

      ref << 12.345;
    }

    assert(read_file_content(file_name) == "12.345");
  }
}

}  // namespace


int main() {
    test_file_stream();
    return 0;
}
