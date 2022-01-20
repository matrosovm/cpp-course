#pragma once

#include <cstddef>

using FuncT = double(*)(double);

double Compose(size_t n, ...);

class Composer {
public:
 	Composer(size_t n, ...);
  	double operator()(double arg) const;
  	~Composer();

private:
	size_t n_;
	FuncT* funcs_;
};