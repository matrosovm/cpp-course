#include <iostream>
#include "growth.hpp"

int main() {
    std::cout << "Stack addresses:" <<std::endl;
    printStackGrowth();

    std::cout << std::endl << "Heap addresses:" <<std::endl;
    printHeapGrowth();
}
