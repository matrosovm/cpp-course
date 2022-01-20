#pragma once

#include <iostream>

class Matrix {
public:
	Matrix();
	Matrix(size_t row, size_t column); 

	void add(const Matrix& mat);
	void mult(const Matrix& mat);
	bool read(const char* file_name);
	void print(std::ostream& os = std::cout) const;
	bool isPossibleAdd(const Matrix& mat) const;
	bool isPossibleMult(const Matrix& mat) const;

	friend void swap(Matrix& left, Matrix& right);

	~Matrix();

private:
	size_t row_;
	size_t column_;
	double** data_;
};
