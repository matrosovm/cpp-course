#include "pretty_print.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <gtest/gtest.h>


template<class T> concept ReallySomehowPrints = requires(const T& t) {
  cls_08::somehow_print(std::cout, t);
};

static const char kNoWay[] = "NO_WAY";

template<class T> // requires (!ReallyPrettyPrints<T>)
std::string print(const T&) {
  return kNoWay;
}

template<ReallySomehowPrints T>
std::string print(const T& t) {
  std::ostringstream ost;
  cls_08::somehow_print(ost, t);
  return ost.str();
}

template<class T>
std::string naive_print(const T& t) {
  std::ostringstream ost;
  ost << t;
  return ost.str();
}

struct hello {
  hello() {}
  virtual ~hello() {}
};

TEST(Somehow, Concepts) {
  EXPECT_TRUE((cls_08::SomehowPrintable<int, true>));
  EXPECT_FALSE((cls_08::SomehowPrintable<int, false>));

  EXPECT_TRUE((cls_08::SomehowPrintable<int***, true>));
  EXPECT_FALSE((cls_08::SomehowPrintable<int***, false>));

  EXPECT_FALSE((cls_08::SomehowPrintable<void***, true>));
  EXPECT_TRUE((cls_08::SomehowPrintable<void***, false>));

  EXPECT_FALSE((cls_08::SomehowPrintable<hello, true>));
  EXPECT_FALSE((cls_08::SomehowPrintable<hello, false>));
}

TEST(Somehow, Prints) {
  int x = 123;
  int* px = &x;
  int** ppx = &px;
  EXPECT_EQ("123", print(x));
  EXPECT_EQ("*123", print(px));
  EXPECT_EQ("**123", print(ppx));

  void* pv = &x;
  void** ppv = &pv;
  EXPECT_EQ(naive_print(pv), print(pv));
  EXPECT_EQ(naive_print(ppv), print(ppv));

  EXPECT_EQ(kNoWay, print(hello{}));
}