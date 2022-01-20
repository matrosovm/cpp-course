#include "cowjson.hpp"

#include <iostream>
#include <sstream>
#include <gtest/gtest.h>

TEST(CowJson, TestEmptyConstruction) {
  cls_04::CowJson json;

  ASSERT_TRUE(json.empty());
}

TEST(CowJson, TestAssignInt) {
  cls_04::CowJson json;
  json = 2;

  cls_04::CowJson other_json = 3;
  json.get<int>() += other_json.get<int>();

  ASSERT_EQ(json.get<int>(), 5);
}

TEST(CowJson, TestIs) {
  cls_04::CowJson json;
  json = 2;

  ASSERT_TRUE(json.is<int>());
  json = "string";
  ASSERT_TRUE(json.is<std::string>());
}

TEST(CowJson, TestCopy) {
  cls_04::CowJson json;
  json = 2;

  cls_04::CowJson other_json = json;

  ASSERT_EQ(other_json.get<int>(), 2);
  other_json.get<int>()++;
  ASSERT_EQ(json.get<int>(), 2);
  ASSERT_EQ(other_json.get<int>(), 3);
}

namespace cls_04::testing {
  template<typename K, typename V>
  struct counting_map : std::map<K, V> {
    using base_t = std::map<K, V>;
    using base_t::map;

    counting_map(const counting_map& other) : base_t(other) {
      copied++;
    }

    counting_map(counting_map&& other) : base_t(std::move(other)) {
      moved++;
    }

    counting_map& operator=(const counting_map& other) {
      base_t::operator=(other);
      if (this != &other) {
        copied++;
      }
      return *this;
    }

    counting_map& operator=(counting_map&& other) {
      base_t::operator=(std::move(other));
      if (this != &other) {
        moved++;
      }
      return *this;
    }

    static size_t copied;
    static size_t moved;
  };

  template<class K, class V>
  size_t counting_map<K, V>::copied = 0;
  template<class K, class V>
  size_t counting_map<K, V>::moved = 0;
}

TEST(CowJson, TestLaziness) {
  using dict = cls_04::CowJson<cls_04::testing::counting_map>::dict;

  cls_04::CowJson<cls_04::testing::counting_map> json = dict{{"some", dict{{"nested", dict{{"value", 1}}}}}};
  cls_04::CowJson<cls_04::testing::counting_map> other_json = json;
  ASSERT_EQ(other_json.get<const dict>().at("some").get<dict>().at("nested").get<dict>().at("value"), 1);
  ASSERT_EQ(dict::copied, 0);
  ASSERT_EQ(dict::moved, 3);

  // we are getting non-const reference, hence copy is unavoidable
  other_json.get<dict>();
  ASSERT_EQ(dict::copied, 1);
  ASSERT_EQ(dict::moved, 3);
}

class DumpTest: public ::testing::TestWithParam<std::pair<std::string, cls_04::CowJson<>>> {};

using dict = cls_04::CowJson<>::dict;
using array = cls_04::CowJson<>::array;

INSTANTIATE_TEST_CASE_P(Dump, DumpTest, ::testing::Values(
        std::pair{"1", 1},
        std::pair{"null", nullptr},
        std::pair{"false", false},
        std::pair{"true", true},
        std::pair{"0.232300", 0.2323},
        std::pair{"112312", 112312},
        std::pair{"112312.232300", 112312.2323},
        std::pair{R"({"Pushkin":"Alexander","some":{"nested":{"value":1},"so":true}})",
         dict{{"some", dict{{"nested", dict{{"value", 1}}}, {"so", true}}}, {"Pushkin", "Alexander"}}},
        std::pair{R"([1,2,{"pi":3.141593}])", array{1, 2, dict{{"pi", 3.1415926}}}},
        std::pair{R"([1,2,true,null,"Vasya"])", array{1,2,true,nullptr,"Vasya"}}
));

TEST_P(DumpTest, Test) {
  auto [expected, json] = GetParam();
  std::stringstream ss;
  json.dump(ss);
  ASSERT_EQ(ss.str(), expected);
}

class RestoreTest: public ::testing::TestWithParam<std::pair<std::string, cls_04::CowJson<>>> {};

TEST_P(RestoreTest, Test) {
  auto [dumped, expected] = GetParam();
  std::stringstream ss;
  ss << dumped;
  cls_04::CowJson restored = cls_04::CowJson<>::restore(ss);
  ASSERT_EQ(expected, restored);
}

INSTANTIATE_TEST_CASE_P(Restore, RestoreTest, ::testing::Values(
        std::pair{"1", 1},
        std::pair{"null", nullptr},
        std::pair{"false", false},
        std::pair{"true", true},
        std::pair{"0.232300", 0.2323},
        std::pair{"112312", 112312},
        std::pair{"112312.232300", 112312.2323},
        std::pair{R"({"Pushkin":"Alexander","some":{"nested":{"value":1},"so":true}})",
                  dict{{"some", dict{{"nested", dict{{"value", 1}}}, {"so", true}}}, {"Pushkin", "Alexander"}}},
        std::pair{R"([1,2,{"pi":3.141593}])", array{1, 2, dict{{"pi", 3.141593}}}},
        std::pair{R"([1,2,true,null,"Vasya"])", array{1,2,true,nullptr,"Vasya"}}
        ));

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
