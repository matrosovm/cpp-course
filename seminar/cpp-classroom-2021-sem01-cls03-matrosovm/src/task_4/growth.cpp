#include <iostream>
#include "growth.hpp"

void printStackVariable() {
	int n;
	std::cout << "second: " << &n << "\n";
}

void printStackGrowth() {
	int n;
	std::cout << "first:  " << &n << "\n";
	printStackVariable();
}

void printHeapVariable() {
	int* n = new int();
	std::cout << "second: " << n << "\n";

	delete n;
}

void printHeapGrowth() {
	int* n = new int();
	std::cout << "first:  " << n << "\n";
	printHeapVariable();

	delete n;
}
