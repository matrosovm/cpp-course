#include <cstdarg>

#include "composition.hpp"

double Compose(size_t n, ...) {
	va_list args;
	va_start(args, n);

	FuncT* funcs = new FuncT[n]();

	for (size_t i = 1; i <= n; ++i)
		funcs[n - i] = va_arg(args, FuncT);

	double result = va_arg(args, double);
	
	for (size_t i = 0; i < n; ++i)
		result = funcs[i](result);

	va_end(args);
	delete[] funcs;

	return result;
}

Composer::Composer(size_t n, ...) : n_(n), funcs_(new FuncT[n]()) {
	va_list args;
	va_start(args, n);

	for (size_t i = 0; i < n_; ++i) 
		funcs_[i] = va_arg(args, FuncT);

	va_end(args);
}

double Composer::operator()(double arg) const {
	for (size_t i = n_ - 1; i != 0; --i)
		arg = funcs_[i](arg);

	return funcs_[0](arg);
}

Composer::~Composer() {
	delete[] funcs_;
}