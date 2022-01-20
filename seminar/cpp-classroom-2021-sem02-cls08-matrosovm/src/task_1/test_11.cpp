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

static const int I = 0;
static const char CS[] = "letters";
static const std::string SS = "string";
static const hello H;
static const hello* const PH = &H;
static const world W;
static const record R {};
static std::shared_ptr<char> PC (new char('?'));
static std::vector<int> VI = {1, 2, 3};

template<cls_08::Outputtable T> bool try_outputtable(const T&) { return true; }
template<class T> bool try_outputtable(const T&) {
  return false; }

TEST(Simply, Outputtable) {
  EXPECT_TRUE(try_outputtable(I));
  EXPECT_FALSE(try_outputtable(PH));
  EXPECT_TRUE(try_outputtable(CS));
  EXPECT_TRUE(try_outputtable(SS));
  EXPECT_FALSE(try_outputtable(H));
  EXPECT_TRUE(try_outputtable(W));
  EXPECT_FALSE(try_outputtable(R));
  EXPECT_FALSE(try_outputtable(PC));
  EXPECT_FALSE(try_outputtable(VI));
}

template<cls_08::Binary T> bool try_binary(const T&) { return true; }
template<class T> bool try_binary(const T&) { return false; }

TEST(Simply, Binary) {
  EXPECT_FALSE(try_binary(I));
  EXPECT_FALSE(try_binary(PH));
  EXPECT_FALSE(try_binary(CS));
  EXPECT_FALSE(try_binary(SS));
  EXPECT_FALSE(try_binary(H));
  EXPECT_FALSE(try_binary(W));
  EXPECT_TRUE(try_binary(R));
  EXPECT_FALSE(try_binary(PC));
  EXPECT_FALSE(try_binary(VI));
}

template<cls_08::PrettyPrintablePointer T> bool try_pointer(const T&) { return true; }
template<class T> bool try_pointer(const T&) { return false; }

TEST(Simple, Pointer) {
  EXPECT_FALSE(try_pointer(I));
  EXPECT_FALSE(try_pointer(PH));
  EXPECT_FALSE(try_pointer(CS));  // сишная строка проходит как outputtable
  EXPECT_FALSE(try_pointer(SS));
  EXPECT_FALSE(try_pointer(H));
  EXPECT_FALSE(try_pointer(W));
  EXPECT_FALSE(try_pointer(R));
  EXPECT_TRUE(try_pointer(PC));
  EXPECT_FALSE(try_pointer(VI));

  EXPECT_TRUE(try_pointer(&I));
  EXPECT_FALSE(try_pointer(&PH));
  EXPECT_TRUE(try_pointer(&CS));
  EXPECT_TRUE(try_pointer(&SS));
  EXPECT_FALSE(try_pointer(&H));
  EXPECT_TRUE(try_pointer(&W));
  EXPECT_TRUE(try_pointer(&R));
  EXPECT_TRUE(try_pointer(&PC));
  // &VI является или не является PrettyPrintablePointer в зависимости от того, поддержали ли мы рекурсию.

  EXPECT_TRUE(try_pointer(dummy_ptr(I)));
  EXPECT_FALSE(try_pointer(dummy_ptr(PH)));
  EXPECT_TRUE(try_pointer(dummy_ptr(CS)));
  EXPECT_TRUE(try_pointer(dummy_ptr(SS)));
  EXPECT_FALSE(try_pointer(dummy_ptr(H)));
  EXPECT_TRUE(try_pointer(dummy_ptr(W)));
  EXPECT_TRUE(try_pointer(dummy_ptr(R)));
  EXPECT_TRUE(try_pointer(dummy_ptr(PC)));
}

template<cls_08::PrettyPrintableCollection T> bool try_collection(const T&) { return true; }
template<class T> bool try_collection(const T&) { return false; }

TEST(Simple, Collection) {
  EXPECT_FALSE(try_collection(I));
  EXPECT_FALSE(try_collection(PH));
  EXPECT_TRUE(try_collection(CS));  // голый массив
  EXPECT_TRUE(try_collection(SS));  // std::string
  EXPECT_FALSE(try_collection(H));
  EXPECT_FALSE(try_collection(W));
  EXPECT_FALSE(try_collection(R));
  EXPECT_FALSE(try_collection(PC));
  EXPECT_TRUE(try_collection(VI));  // std::vector

  EXPECT_TRUE(try_collection(std::vector{I}));
  EXPECT_FALSE(try_collection(std::vector{PH}));
  EXPECT_TRUE(try_collection(std::vector{CS}));
  EXPECT_TRUE(try_collection(std::vector{SS}));
  EXPECT_FALSE(try_collection(std::vector{H}));
  EXPECT_TRUE(try_collection(std::vector{W}));
  EXPECT_TRUE(try_collection(std::vector{R}));
  EXPECT_TRUE(try_collection(std::vector{PC}));
  EXPECT_TRUE(try_collection(std::vector{VI}));

  EXPECT_TRUE(try_collection(std::vector{dummy_ptr(I)}));
  EXPECT_FALSE(try_collection(std::vector{dummy_ptr(PH)}));
  EXPECT_TRUE(try_collection(std::vector{dummy_ptr(CS)}));
  EXPECT_TRUE(try_collection(std::vector{dummy_ptr(SS)}));
  EXPECT_FALSE(try_collection(std::vector{dummy_ptr(H)}));
  EXPECT_TRUE(try_collection(std::vector{dummy_ptr(W)}));
  EXPECT_TRUE(try_collection(std::vector{dummy_ptr(R)}));
  EXPECT_TRUE(try_collection(std::vector{dummy_ptr(PC)}));
  EXPECT_TRUE(try_collection(std::vector{dummy_ptr(VI)}));
}

template<cls_08::PrettyPrintable T> bool try_printable(const T&) { return true; }
template<class T> bool try_printable(const T&) { return false; }

TEST(Simple, Printable) {
  EXPECT_TRUE(try_printable(I));
  EXPECT_FALSE(try_printable(PH));
  EXPECT_TRUE(try_printable(CS));
  EXPECT_TRUE(try_printable(SS));
  EXPECT_FALSE(try_printable(H));  // этот тип и его производные мы не можем вывести никак
  EXPECT_TRUE(try_printable(W));
  EXPECT_TRUE(try_printable(R));
  EXPECT_TRUE(try_printable(PC));
  EXPECT_TRUE(try_printable(VI));

  EXPECT_TRUE(try_printable(dummy_ptr(I)));
  EXPECT_FALSE(try_printable(dummy_ptr(PH)));
  EXPECT_TRUE(try_printable(dummy_ptr(CS)));
  EXPECT_TRUE(try_printable(dummy_ptr(SS)));
  EXPECT_FALSE(try_printable(dummy_ptr(H)));
  EXPECT_TRUE(try_printable(dummy_ptr(W)));
  EXPECT_TRUE(try_printable(dummy_ptr(R)));
  EXPECT_TRUE(try_printable(dummy_ptr(PC)));

  EXPECT_TRUE(try_printable(std::vector{I}));
  EXPECT_FALSE(try_printable(std::vector{PH}));
  EXPECT_TRUE(try_printable(std::vector{CS}));
  EXPECT_TRUE(try_printable(std::vector{SS}));
  EXPECT_FALSE(try_printable(std::vector{H}));
  EXPECT_TRUE(try_printable(std::vector{W}));
  EXPECT_TRUE(try_printable(std::vector{R}));
  EXPECT_TRUE(try_printable(std::vector{PC}));
}
