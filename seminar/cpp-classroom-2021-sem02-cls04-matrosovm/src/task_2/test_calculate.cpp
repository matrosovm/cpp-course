#include "calculate.hpp"

#include <string>
#include <vector>
#include <gtest/gtest.h>


TEST(calculate, BasicOperations) {
    std::vector<std::string> expressions = {"1+2",
                                            "9-6",
                                            "3*1",
                                            "12/4"};
    for (auto& expr : expressions) {
        ASSERT_EQ(calculate(expr), 3);
    }
}

TEST(calculate, RemovingSpaces) {
    std::vector<std::string> expressions = {"     1 +         2",
                                            "    9 -   6  ",
                                            "   3  *  1 ",
                                            "   12     /    4   "};
    for (auto& expr : expressions) {
        ASSERT_EQ(calculate(expr), 3);
    }
}

TEST(calculate, Brackets) {
    std::vector<std::string> expressions = {"(1 + 2) / 3 * 4",
                                            "(4 + 11 * (2 + 2)) / (3 * 4)",
                                            "(55 / (12 - 1) * 2 - 6)"};
    for (auto& expr : expressions) {
        ASSERT_EQ(calculate(expr), 4);
    }
}

TEST(calculate, FloatingNumbers) {
    std::vector<std::string> expressions = {"(1 + 2.5) / 3.5 * 4",
                                            "(2.5 - 1.25) * 3 - 0.25 + 0.5",
                                            "6.72e2 / 0.168E3"};
    for (auto& expr : expressions) {
        ASSERT_EQ(calculate(expr), 4);
    }
}

TEST(calculate, UnaryPlusMinus) {
    std::vector<std::string> expressions = {"+----4",
                                            "(1 + (----4) +-0 ---1)",
                                            "---0+++0----0+4-----------0"};
    for (auto& expr : expressions) {
        ASSERT_EQ(calculate(expr), 4);
    }
}

TEST(calculate, Exception) {
    std::vector<std::string> expressions = {"(+(----4)",
                                            "(1 + (----4 / 0) +-0 ---1)",
                                            "---0+++0--()--0+4---(--)------0",
                                            "0 + 0/(0*0)"};
    for (auto& expr : expressions) {
        try {
            ASSERT_EQ(calculate(expr), 4);
            ASSERT_TRUE(false);
        } catch(calculate_exception& ex) {
        }
    }
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}