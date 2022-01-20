#include <iostream>

#include "filesystem.hpp"
#include "disk_utils.hpp"
#include "big.hpp"
#include "bad_lib.hpp"

int main() {
    {
        filesystem::File f("a");
        //    f = filesystem::createFile("b");
        std::cout << f.getPath() << '\n';
        std::cout << disk_utils::du("") << '\n';
        std::cout << disk_utils::du(f) << '\n';
        std::cout << big::someFunc() << '\n';
    }

    {
        bad_lib::Cat ethalon{bad_lib::kFemale, 1, 1};
        bad_lib::Cat cat = bad_lib::bornACat(bad_lib::kFemale);
        std::cout << ToStringInHeader(cat) << '\n';
        std::cout << ToStringInCpp(cat) << '\n';
        cat = bad_lib::anotherYearOfLive(cat);
        cat = bad_lib::reincarnateACat(cat);
        cat = bad_lib::anotherYearOfLive(cat);
        std::cout << ToStringInHeader(cat) << '\n';
        std::cout << ToStringInCpp(cat) << '\n';

        std::cout << ToStringInHeader(ethalon) << '\n';
        std::cout << ToStringInCpp(ethalon) << '\n';
    }
    return 0;
}
