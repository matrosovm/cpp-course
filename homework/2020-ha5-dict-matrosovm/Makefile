# Basic make file.

all: smoke

smoke_test: smoke_test.cpp dict.hpp dict_serialization.hpp dict_json.hpp
	$(CXX) -g -Wall -Wextra -std=c++2a -o smoke_test smoke_test.cpp

smoke: smoke_test
	./smoke_test

all: smoke
