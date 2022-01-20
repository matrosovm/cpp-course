#include "phonebook.hpp"
#include "unix/dlloader.hpp"

#include <cassert>
#include <stdexcept>
#include <vector>
#include <gtest/gtest.h>

class PhoneBookTest : public ::testing::TestWithParam<control_02::PhoneBook> {
 protected:
  PhoneBookTest() {
#if defined(__linux__) || defined(__APPLE__)
    format_to_lib["json"] = lib_dir + "/libjson-parser.so";
    format_to_lib["protobuf"] = lib_dir + "/libprotobuf-parser.so";
#endif

#ifdef WIN32
    // if running on windows, change this to actual dll paths, not sure these are correct
    format_to_lib["json"] = PARSER_LIBRARY_DIR + "\\libjson-parser.dll";
    format_to_lib["protobuf"] = PARSER_LIBRARY_DIR + "\\libprotobuf-parser.dll";
#endif
  }
  std::map<std::string, std::string> format_to_lib;
  std::string lib_dir = PARSER_LIBRARY_DIR;

  void test_write(const std::string& format) {
    auto original_pb = GetParam();
    std::stringstream ss;

    control_02::dlloader::DLLoader loader(format_to_lib[format]);

    using readtype = void(*)(control_02::PhoneBook*, std::istream&);
    using writetype = void(*)(const control_02::PhoneBook*, std::istream&);

    auto read_phonebook = loader.get_func<readtype>("read_phonebook");
    auto write_phonebook = loader.get_func<writetype>("write_phonebook");
    write_phonebook(&original_pb, ss);
    control_02::PhoneBook restored;

    read_phonebook(&restored, ss);

    ASSERT_EQ(original_pb, restored);
  }
};


TEST_P(PhoneBookTest, TestWriteJson) {
  test_write("json");
}

TEST_P(PhoneBookTest, TestWriteProtobuf) {
  test_write("protobuf");
}


INSTANTIATE_TEST_SUITE_P(PhoneBookTest, PhoneBookTest, ::testing::Values(
        control_02::PhoneBook{},
        control_02::PhoneBook{{{"name1", "123456"}, {"name2", "535353"}}}

));


int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
