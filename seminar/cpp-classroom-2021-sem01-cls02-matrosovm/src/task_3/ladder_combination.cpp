#include "ladder_combination.hpp"

#include <cstddef>

uint64_t ladderCombinations(uint8_t steps) {
	static size_t countedUpTo = 2;
	static uint64_t ways[21] {1, 1};

	for (; countedUpTo <= steps; ++countedUpTo)
		ways[countedUpTo] = ways[countedUpTo - 1] + ways[countedUpTo - 2];  
	
	return ways[steps]; 
}