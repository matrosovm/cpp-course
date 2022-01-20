#include "sum.hpp"

#include <iostream>
#include <dlfcn.h>
#include <cassert>

void testSum() {
    assert(sum(2, 3) == 5);
    assert(sum(0, 1) == 1);
    assert(sum(0, 0) == 0);
    assert(sum(2, -30) == -28);
}

void testMul(int (*mul)(int, int)) {
    assert(mul(2, 3) == 6);
    assert(mul(0, 1) == 0);
    assert(mul(0, 0) == 0);
    assert(mul(2, -30) == -60);
}

int main() {
    void* mul_library;
    mul_library = dlopen("libmul.so",RTLD_LAZY);
    if (!mul_library){
        std::cerr << "Cannot open mul library " << dlerror() << std::endl;
        return 1;
    };
    dlerror();
    auto mul = reinterpret_cast<int (*)(int, int)>(dlsym(mul_library, "mul"));
    {
        auto error = dlerror();
        if (error) {
            std::cerr << "Cannot find mul " << error << std::endl;
            dlclose(mul_library);
            return 1;
        }
    }
    testSum();
    testMul(mul);
    std::cout << "all tests passed" << std::endl;
    dlclose(mul_library);
    return 0;
}
