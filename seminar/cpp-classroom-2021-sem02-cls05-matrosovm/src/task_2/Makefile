all: test
	./test.out

test: test.cpp libsum.a libmul.so
	g++ test.cpp -L. -Wl,-rpath . -ldl -lsum -o test.out

libsum.a: sum.cpp sum.hpp
	g++ -c sum.cpp -o sum.o && \
	ar crv libsum.a sum.o && \
	rm sum.o

libmul.so: mul.cpp mul.hpp
	g++ -fPIC -shared mul.cpp -o libmul.so

clean:
	rm *.so *.a *.out