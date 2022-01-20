.PHONY: all clear

all: sum test.o sum.o

sum: sum.o test.o
	clang++ sum.o test.o -o sum

sum.o: sum.c
	clang -c $<  
test.o: test.cpp
	clang++ -c $< 

clear:
	rm -rf *.o sum