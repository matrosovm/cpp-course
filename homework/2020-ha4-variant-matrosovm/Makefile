# Basic make file.

all: smoke

variant_smoke_test: variant_smoke_test.cpp variant.hpp catch.hpp
	$(CXX) -g -Wall -Wextra -std=c++17 -o variant_smoke_test variant_smoke_test.cpp

visitor_smoke_test: visitor_smoke_test.cpp variant.hpp catch.hpp
	$(CXX) -g -Wall -Wextra -std=c++17 -o visitor_smoke_test visitor_smoke_test.cpp

smoke: variant_smoke_test visitor_smoke_test
	./variant_smoke_test && ./visitor_smoke_test

all: smoke
