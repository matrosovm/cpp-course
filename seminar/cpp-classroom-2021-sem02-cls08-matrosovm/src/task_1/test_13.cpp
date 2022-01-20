#include "pretty_print.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <gtest/gtest.h>


template<class T> concept ReallyPrettyPrints = requires(const T& t) {
  cls_08::pretty_print(std::cout, t);
};

static const char kNotPretty[] = "NOT_PRETTY";

template<class T> // requires (!ReallyPrettyPrints<T>)
std::string print(const T&) {
  return kNotPretty;
}

template<ReallyPrettyPrints T>
std::string print(const T& t) {
  std::ostringstream ost;
  cls_08::pretty_print(ost, t);
  return ost.str();
}


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
  unsigned char a, b, c, d;
};

template<class T> struct dummy_ptr {
  dummy_ptr() {}
  explicit dummy_ptr(T) {}
  operator T*() const { static T t; return &t; }
};

TEST(Printable,Pretty) {
  EXPECT_TRUE(cls_08::PrettyPrintable<decltype(123)>);
}

TEST(Print, Pretty) {
  EXPECT_EQ("123", print(123));
  EXPECT_EQ("<hello, world>", print(world{}));
  EXPECT_EQ("01,02,03,04", print(record{1,2,3,4}));
  EXPECT_EQ("abc", print((const char*)"abc"));
  EXPECT_EQ("nullptr", print((int*)nullptr));
  int x = 123;
  EXPECT_EQ("*123", print(&x));
  EXPECT_EQ("{ *123 *123 *123 }", print(std::vector{&x, &x, &x}));
  std::vector v{1, 2, 3};
  EXPECT_EQ("{ 1 2 3 }", print(v));
  EXPECT_EQ("{ a b c }", print(std::vector{'a', 'b', 'c'}));
  EXPECT_EQ("abc", print(std::string("abc")));  // строка - хоть и коллекция, но вывести её надо как строку
}

TEST(Print, Pretty2) {
  std::vector v{1, 2, 3};
  EXPECT_EQ("*{ 1 2 3 }", print(&v));
  EXPECT_EQ("{ { 1 2 3 } { 1 2 3 } }", print(std::vector{v, v}));
}


TEST(Print, NotPretty) {
  EXPECT_EQ(kNotPretty, print((void*)nullptr));
  EXPECT_EQ(kNotPretty, print(hello{}));
  EXPECT_EQ(kNotPretty, print(dummy_ptr{hello{}}));
}