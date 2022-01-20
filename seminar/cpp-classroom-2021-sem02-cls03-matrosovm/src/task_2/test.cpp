#include "cchecks.hpp"
#include "ctest.hpp"

#include <cassert>
#include <sstream>

#define LVL1
#define LVL2
#define LVL3

namespace {
int f(int x, int y) { return x ^ y; }

std::vector<std::string> &expected_lines() {
    static std::vector<std::string> v;
    return v;
}

void add_successful(const std::string &case_name,
                    const std::string &test_name) {
    expected_lines().push_back("[      OK  ] " + case_name + "." + test_name);
}

void add_failed(const std::string &case_name, const std::string &test_name) {
    expected_lines().push_back("[  FAILED  ] " + case_name + "." + test_name);
}

}// namespace

#ifdef LVL1

O_TEST(Lvl1, success) {
    O_ASSERT_EQ(f(5, 10), 15);
    int x, y;
    O_ASSERT_EQ(x = 5, y = 5);
    O_ASSERT_NEQ(x = 5, y = 7);
}

int state_changing_function() {
    static int x = 0;
    return x++;
}

int line1, line2;

O_TEST(Lvl1, failure) {
    line1 = __LINE__; O_EXPECT_EQ(f(5, 10), 16);
    line2 = __LINE__; O_EXPECT_EQ(state_changing_function(), 1);
}

O_TEST(Lvl1_1, new_case) { O_EXPECT_EQ(f(7, 8), 15); }

void fill_expected_lines_lvl1() {
    add_successful("Lvl1", "success");
    add_successful("Lvl1_1", "new_case");
    add_failed("Lvl1", "failure");
    expected_lines().emplace_back("main.cpp:" + std::to_string(line1) + ": Failure");
    expected_lines().emplace_back("main.cpp:" + std::to_string(line2) + ": Failure");
}

#endif

#ifdef LVL2

O_TEST(Lvl2, success) {
    O_ASSERT_FLOAT_EQ(1e5, 1e5 + 1e-6);
    O_ASSERT_FLOAT_NEQ(1e5, 1e5 + 1);

    O_ASSERT_TRUE(true | false);
    O_ASSERT_FALSE(true & false);

    O_EXPECT_THROW([] { throw 4; }(), int);
    O_EXPECT_NO_THROW([] {}());
}

O_TEST(Lvl2, failure1) {
    O_EXPECT_THROW([] { ; }(), std::exception);
}

O_TEST(Lvl2, failure2) {
    O_EXPECT_THROW([] { throw 4; }(), std::exception);
}

O_TEST(Lvl2, failure3) {
    O_EXPECT_NO_THROW([] { throw 4; }());
}

void fill_expected_lines_lvl2() {
    add_successful("Lvl2", "success");
    add_failed("Lvl2", "failure1");
    add_failed("Lvl2", "failure2");
    add_failed("Lvl2", "failure3");
}

#endif

#ifdef LVL3

class DbConnection {
 public:
    explicit DbConnection(std::string conn) : conn(std::move(conn)) {}

 private:
    std::string conn;
};

class HttpClient {};

class Service {
 public:
    Service(DbConnection conn, HttpClient client)
        : conn_(std::move(conn)), client_(std::move(client)) {}

    int ping() const { return 200; }

    int putEntity(const std::string &) const { return 403; }

 private:
    DbConnection conn_;
    HttpClient client_;
};

class ServiceFixture : public Test {
 public:
    static inline int setup = 0;
    static inline int teardown = 0;

 protected:
    ServiceFixture()
        : client_(), conn_("localhost"), service_(conn_, client_) {}

    void SetUp() override { setup++; }

    void TearDown() override { teardown++; }

    HttpClient client_;
    DbConnection conn_;
    Service service_;
};

O_TEST_F(ServiceFixture, ping) { O_EXPECT_EQ(service_.ping(), 200); }

O_TEST_F(ServiceFixture, putEntity) {
    O_EXPECT_EQ(service_.putEntity("Jon Snow"), 200);
}

void fill_expected_lines_lvl3() {
    add_successful("ServiceFixture", "ping");
    add_failed("ServiceFixture", "putEntity");
}

#endif

void check_required_strings(const std::string &log) {
    for (const auto &str : expected_lines()) {
        if (log.find(str) == std::string::npos) {
            std::cout << "I was expecting the line " << str << '\n';
            std::cout << "Full log is\n" << log << '\n';
            assert(false);
        }
    }
}

int main() {
    std::stringstream ss;
    run_all_tests(ss);

    int test_cases = 0;
    int tests = 0;

#ifdef LVL1
    fill_expected_lines_lvl1();

    // function should've been called exactly once
    assert(state_changing_function() == 1);
    test_cases += 2;
    tests += 3;
#endif

#ifdef LVL2
    fill_expected_lines_lvl2();
    test_cases++;
    tests += 4;
#endif

#ifdef LVL3
    fill_expected_lines_lvl3();
    assert(ServiceFixture::setup == 2);
    assert(ServiceFixture::teardown == 2);
    test_cases++;
    tests += 2;
#endif
    expected_lines().push_back("Running " + std::to_string(tests)
                               + " tests from " + std::to_string(test_cases)
                               + " test cases");

    check_required_strings(ss.str());
}
