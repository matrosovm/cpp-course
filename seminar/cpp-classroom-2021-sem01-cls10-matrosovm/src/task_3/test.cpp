#include "event.hpp"
#include <iostream>
#include <cassert>


void print(const void *a) {
    std::cout << static_cast<const char*>(a) << std::endl;
}

void test_prints() {
    cls10::Event<void(const char*)> ev;

    ev.subscribe(print);
    ev.subscribe([](const char* c) { std::cout << "lambda " << c << std::endl; });

    ev.fire("hi!");
}

int globalI;
double globalD;

int subOne(int i) {
    globalI = i - 1;
    return globalI;
}


int addWithCast(double d) {
    globalD = d + 10;
    return globalD;
}

void test_counts() {
    cls10::Event<int(int)> ev;

    ev.subscribe(subOne);
    ev.subscribe(addWithCast);
    ev.fire(42);

    assert(globalI == 41);
    assert(globalD == 52.);
}


int main() {
    test_prints();
    test_counts();

    return 0;
}
