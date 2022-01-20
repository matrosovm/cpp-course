#include <string_view>

#include "matrices.hpp"

void printOperationError(const Matrix& left, const Matrix& right, const std::string_view& sv) {
	std::cerr << "Unable to " << sv << " matrices:" << std::endl;
	left.print(std::cerr);
	right.print(std::cerr);	
}

int main(int argc, char** argv) {
	Matrix left{};

	if (argc % 2 != 0) {
		std::cerr << "Invalid count of arguments" << std::endl;
		return -1;
	} else if (!left.read(argv[1])) {
		std::cerr << "Unable to open file " << argv[1] << std::endl;
		return -2;
	}
	for (int i = 2; i < argc; i += 2) {
		std::string_view sv(argv[i]);

		if (sv != "--add" && sv != "--mult") {
	    	std::cerr << "Invalid argument " << argv[i] << std::endl; 
	    	return -4;
	   	}  
		
		Matrix right; 
		if (!right.read(argv[i + 1])) {
			std::cerr << "Unable to open file " << argv[i + 1] << std::endl;
			return -2;
		}
		
		if ((sv == "--add" && !left.isPossibleAdd(right)) || 
			(sv == "--mult" && !left.isPossibleMult(right))) {
			printOperationError(left, right, sv.substr(2));
			return -3;
		}
		if (sv == "--add")
			left.add(right);
		else
			left.mult(right);
	}

	left.print();
	return 0;
}