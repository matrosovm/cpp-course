#include "pretty_print.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <gtest/gtest.h>


// non-outputtable, non-binary
struct hello {
  hello() {}
  virtual ~hello() {}
};

// outputtable
struct world : hello {
  friend std::ostream& operator << (std::ostream& ost, const world& v) { return ost << "<hello, world>"; }
};

struct record {
  int x, y;
};

template<class T> struct dummy_ptr {
  dummy_ptr() {}
  explicit dummy_ptr(T) {}
  operator T*() const { static T t; return &t; }
};


template<cls_08::PrettyPrintable T> bool try_printable(const T&) { return true; }
template<class T> bool try_printable(const T&) { return false; }

TEST(Recursive, Pointer) {
  EXPECT_TRUE(try_printable((int*****)nullptr));
  EXPECT_TRUE(try_printable((int const* const* const* const* const*)nullptr));
  EXPECT_TRUE(try_printable((std::shared_ptr<std::shared_ptr<std::shared_ptr<std::shared_ptr<int>>>>())));
  EXPECT_TRUE(try_printable((char****)nullptr));
  EXPECT_FALSE(try_printable((hello*)nullptr));
  EXPECT_FALSE(try_printable((hello****)nullptr));
  EXPECT_FALSE(try_printable((std::shared_ptr<std::shared_ptr<std::shared_ptr<std::shared_ptr<hello>>>>())));
  EXPECT_TRUE(try_printable((world*****) nullptr));
  EXPECT_TRUE(try_printable((record*****) nullptr));
}

TEST(Recursive, Collection) {
  EXPECT_TRUE(try_printable(std::vector{std::vector{std::vector{1}}}));
  EXPECT_FALSE(try_printable(std::vector{std::vector{std::vector{hello{}}}}));
}

TEST(Recursive, Mixed) {
  EXPECT_TRUE(try_printable(std::vector{dummy_ptr{std::vector{1}}}));
  EXPECT_FALSE(try_printable(std::vector{dummy_ptr{std::vector{hello{}}}}));

}