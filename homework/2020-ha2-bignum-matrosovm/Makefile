# Basic make file.

all: smoke

smoke_test: smoke_test.cpp bignum.hpp
	$(CXX) -g -Wall -Wextra -std=c++17 -o smoke_test smoke_test.cpp

smoke: smoke_test
	./smoke_test

all: smoke
