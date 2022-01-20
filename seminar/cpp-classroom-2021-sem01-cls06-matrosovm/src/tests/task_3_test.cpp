#include <functional>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cassert>
#include "my_ostream.hpp"
#include "my_ostream_con.hpp"
#include "my_ostream_file.hpp"
#include "my_ostream_combo.hpp"

namespace {

void test_with_replaced_cout(const std::function<void(const std::ostringstream &sout)> &action) {
  std::ostringstream buffer;
  std::streambuf *coutbuf = std::cout.rdbuf();
  std::cout.rdbuf(buffer.rdbuf());

  action(buffer);

  std::cout.rdbuf(coutbuf);
}

std::string read_file_content(const std::string &name) {
  std::fstream file{name};

  return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

void test_combo_stream() {
  const char *f1_file = "f1.txt";
  const char *f2_file = "f2.txt";

  auto clear_files = [=]() {
    std::remove(f1_file);
    std::remove(f2_file);
  };

  {
    clear_files();

    {
      cls_06::my_ostream_combo combo{new cls_06::my_ostream_file{f1_file},
                                     new cls_06::my_ostream_file{f2_file}};
      combo << "Hello!" << 1 << " " << 2.3;
    }

    assert(read_file_content(f1_file) == read_file_content(f2_file));
    assert(read_file_content(f1_file) == "Hello!1 2.3");
  }

  {
    clear_files();

    test_with_replaced_cout([=](auto const &sout) {
      {
        cls_06::my_ostream_combo combo{
          new cls_06::my_ostream_combo{
            new cls_06::my_ostream_con{},
            new cls_06::my_ostream_file{f1_file}
          },
          new cls_06::my_ostream_file{f2_file}
        };

        combo << "Hello!" << 1 << " " << 2.3;
      }

      assert(read_file_content(f1_file) == read_file_content(f2_file));
      assert(read_file_content(f2_file) == sout.str());
    });
  }
}

} // namespace


int main() {
    test_combo_stream();

    return 0;
}
